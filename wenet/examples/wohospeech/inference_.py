# Copyright (c) 2020 Mobvoi Inc. (authors: Binbin Zhang, Xiaoyu Chen, Di Wu)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from __future__ import print_function

import argparse
import logging
import sys

import yaml
import torch
import torchaudio
import torchaudio.compliance.kaldi as kaldi
from time import time

from wenet.transformer.asr_model import init_asr_model

import wenet.utils.weight_dump as weight_dump

def get_args():
    parser = argparse.ArgumentParser(description='recognize with your model')
    parser.add_argument('--config', required=True, help='config file')
    parser.add_argument('--test_data', required=True, help='test data file')
    parser.add_argument('--data_type',
                        default='raw',
                        choices=['raw', 'shard'],
                        help='train and cv data type')
    parser.add_argument('--gpu',
                        type=int,
                        default=-1,
                        help='gpu id for this rank, -1 for cpu')
    parser.add_argument('--checkpoint', required=True, help='checkpoint model')
    parser.add_argument('--dict', required=True, help='dict file')
    parser.add_argument('--beam_size',
                        type=int,
                        default=10,
                        help='beam size for search')
    parser.add_argument('--penalty',
                        type=float,
                        default=0.0,
                        help='length penalty')
    parser.add_argument('--result_file', required=True, help='asr result file')
    parser.add_argument('--batch_size',
                        type=int,
                        default=16,
                        help='asr result file')
    parser.add_argument('--mode',
                        choices=[
                            'attention', 'ctc_greedy_search',
                            'ctc_prefix_beam_search', 'attention_rescoring'
                        ],
                        default='attention',
                        help='decoding mode')
    parser.add_argument('--ctc_weight',
                        type=float,
                        default=0.0,
                        help='ctc weight for attention rescoring decode mode')
    parser.add_argument('--decoding_chunk_size',
                        type=int,
                        default=-1,
                        help='''decoding chunk size,
                                <0: for decoding, use full chunk.
                                >0: for decoding, use fixed chunk size as set.
                                0: used for training, it's prohibited here''')
    parser.add_argument('--num_decoding_left_chunks',
                        type=int,
                        default=-1,
                        help='number of left chunks for decoding')
    parser.add_argument('--simulate_streaming',
                        action='store_true',
                        help='simulate streaming inference')
    parser.add_argument('--reverse_weight',
                        type=float,
                        default=0.0,
                        help='''right to left weight for attention rescoring
                                decode mode''')

    args = parser.parse_args()
    print(args)
    return args


def main():
    args = get_args()
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(levelname)s %(message)s')

    if args.mode in ['ctc_prefix_beam_search', 'attention_rescoring'
                     ] and args.batch_size > 1:
        logging.fatal(
            'decoding mode {} must be running with batch_size == 1'.format(
                args.mode))
        sys.exit(1)

    with open(args.config, 'r') as fin:
        configs = yaml.load(fin, Loader=yaml.FullLoader)

    waveform, sample_rate = torchaudio.load(args.test_data)
    waveform = waveform * (1 << 15)
    print(waveform.dtype)
    # Only keep key, feat, label
    mat = kaldi.fbank(waveform,
                        num_mel_bins=80,
                        frame_length=25,
                        frame_shift=10,
                        dither=0.0,
                        energy_floor=0.0,
                        sample_frequency=sample_rate)
    print(mat.dtype)
    feats = mat.view(1, -1, 80)
    feats_length = torch.tensor([mat.size(0)], dtype=torch.int32)
    targets = torch.tensor([[2474, 3116,  331, 2408,   82, 1684,  321,   47,  235, 2199, 2553, 1319, 307]])
    targets_length = torch.tensor([13], dtype=torch.int32)

    # Init asr model from configs
    model = init_asr_model(configs)
    # print(model)
    with open('asr_model', 'w') as fout_model:
        fout_model.write('{}\n'.format(model))
    num_params = sum(p.numel() for p in model.parameters())
    print('the number of model params: {}'.format(num_params))

    # Load dict
    char_dict = {}
    with open(args.dict, 'r') as fin:
        for line in fin:
            arr = line.strip().split()
            assert len(arr) == 2
            char_dict[int(arr[1])] = arr[0]
    eos = len(char_dict) - 1

    use_cuda = args.gpu >= 0 and torch.cuda.is_available()
    if use_cuda:
        logging.info('Checkpoint: loading from checkpoint %s for GPU' % args.checkpoint)
        checkpoint = torch.load(args.checkpoint)
    else:
        logging.info('Checkpoint: loading from checkpoint %s for CPU' % args.checkpoint)
        checkpoint = torch.load(args.checkpoint, map_location='cpu')
    start_ = time()
    model.load_state_dict(checkpoint)
    print('model load time: {}'.format(time() - start_))
    device = torch.device('cuda' if use_cuda else 'cpu')
    model = model.to(device)

    weight_dump.conv2d_dump(model.encoder.embed.conv[0],'model_dump/Subsampling4_Conv_Conv2D_1')
    weight_dump.conv2d_dump(model.encoder.embed.conv[2],'model_dump/Subsampling4_Conv_Conv2D_2')
    weight_dump.linear_dump(model.encoder.embed.out[0],'model_dump/Subsampling4_Out_Linear')

    for layer_num in range(0,12):
        weight_dump.layernorm_dump(model.encoder.encoders[layer_num].norm_ff_macaron,'model_dump/Conformer'+str(layer_num)+'_norm_ff_macaron')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].feed_forward_macaron.w_1,'model_dump/Conformer'+str(layer_num)+'_feed_forward_macaron_w_1')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].feed_forward_macaron.w_2,'model_dump/Conformer'+str(layer_num)+'_feed_forward_macaron_w_2')

        weight_dump.layernorm_dump(model.encoder.encoders[layer_num].norm_mha,'model_dump/Conformer'+str(layer_num)+'_norm_mha')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].self_attn.linear_q,'model_dump/Conformer'+str(layer_num)+'_self_attn_linear_q')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].self_attn.linear_k,'model_dump/Conformer'+str(layer_num)+'_self_attn_linear_k')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].self_attn.linear_v,'model_dump/Conformer'+str(layer_num)+'_self_attn_linear_v')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].self_attn.linear_out,'model_dump/Conformer'+str(layer_num)+'_self_attn_linear_out')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].self_attn.linear_pos,'model_dump/Conformer'+str(layer_num)+'_self_attn_linear_pos',False)
        weight_dump.param_dump(model.encoder.encoders[layer_num].self_attn.pos_bias_u,'model_dump/Conformer'+str(layer_num)+'_self_attn_pos_bias_u')
        weight_dump.param_dump(model.encoder.encoders[layer_num].self_attn.pos_bias_v,'model_dump/Conformer'+str(layer_num)+'_self_attn_pos_bias_v')

        weight_dump.layernorm_dump(model.encoder.encoders[layer_num].norm_conv,'model_dump/Conformer'+str(layer_num)+'_norm_conv')
        weight_dump.layernorm_dump(model.encoder.encoders[layer_num].norm_final,'model_dump/Conformer'+str(layer_num)+'_norm_final')

        weight_dump.conv1d_dump(model.encoder.encoders[layer_num].conv_module.pointwise_conv1,'model_dump/Conformer'+str(layer_num)+'_conv_module_pointwise_conv1')
        weight_dump.conv1d_dump(model.encoder.encoders[layer_num].conv_module.depthwise_conv,'model_dump/Conformer'+str(layer_num)+'_conv_module_depthwise_conv')
        weight_dump.layernorm_dump(model.encoder.encoders[layer_num].conv_module.norm,'model_dump/Conformer'+str(layer_num)+'_conv_module_norm')
        weight_dump.conv1d_dump(model.encoder.encoders[layer_num].conv_module.pointwise_conv2,'model_dump/Conformer'+str(layer_num)+'_conv_module_pointwise_conv2')

        weight_dump.layernorm_dump(model.encoder.encoders[layer_num].norm_ff,'model_dump/Conformer'+str(layer_num)+'_norm_ff')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].feed_forward.w_1,'model_dump/Conformer'+str(layer_num)+'_feed_forward_w_1')
        weight_dump.linear_dump(model.encoder.encoders[layer_num].feed_forward.w_2,'model_dump/Conformer'+str(layer_num)+'_feed_forward_w_2')

    weight_dump.layernorm_dump(model.encoder.after_norm,'model_dump/Conformer_after_norm')
    weight_dump.linear_dump(model.ctc.ctc_lo,'model_dump/CTC_ctc_lo')
    # print(model.ctc.ctc_lo)

    model.eval()
    with torch.no_grad(), open(args.result_file, 'w') as fout:
        feats = feats.to(device)
        print(feats)
        print(feats.size())
        feats_length = feats_length.to(device)
        print(feats_length)
        print(feats_length.size())
        if args.mode == 'ctc_prefix_beam_search':
            assert (feats.size(0) == 1)
            start = time()
            hyp, _ = model.ctc_prefix_beam_search(
                feats,
                feats_length,
                args.beam_size,
                decoding_chunk_size=args.decoding_chunk_size,
                num_decoding_left_chunks=args.num_decoding_left_chunks,
                simulate_streaming=args.simulate_streaming)
            print('model inference time: {}'.format(time() - start))
            hyps = [hyp]
        elif args.mode == 'attention_rescoring':
            assert (feats.size(0) == 1)
            start = time()
            hyp, _ = model.attention_rescoring(
                feats,
                feats_length,
                args.beam_size,
                decoding_chunk_size=args.decoding_chunk_size,
                num_decoding_left_chunks=args.num_decoding_left_chunks,
                ctc_weight=args.ctc_weight,
                simulate_streaming=args.simulate_streaming,
                reverse_weight=args.reverse_weight)
            print('model inference time: {}'.format(time() - start))
            hyps = [hyp]
        
        content = ''
        print(hyps[0])
        for w in hyps[0]:
            if w == eos:
                break
            content += char_dict[w]
        logging.info('{}'.format(content))
        fout.write('{}\n'.format(content))

if __name__ == '__main__':
    main()
