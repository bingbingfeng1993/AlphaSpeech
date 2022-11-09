#ifndef __ASR_DECODER_H__
#define __ASR_DECODER_H__

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/time.h>

#include "../src/core/mat.h"
#include "../src/core/permute.h"

#include "../src/utils/fbank.h"
#include "../src/utils/ctc_endpoint.h"
#include "../src/utils/ctc_prefix_beam_search.h"
#include "../src/utils/ctc_prefix_beam_search_wrt_timestamp.h"
#include "../src/utils/inverse_text_normalization.h"
#include "../src/utils/post_processor.h"

#include "../src/layer/conv1d.h"
#include "../src/layer/conv2d.h"
#include "../src/layer/depthwise_conv1d.h"
#include "../src/layer/glu.h"
#include "../src/layer/layernorm.h"
#include "../src/layer/linear.h"
#include "../src/layer/parameter.h"
#include "../src/layer/relu.h"
#include "../src/layer/silu.h"
#include "../src/layer/softmax.h"

#define num_bins        80
#define sample_rate     16000
#define frame_length    400
#define frame_shift     160

#define num_encoder_layer 12
#define k_v_cache_max 125

#define unit_byte_max 20

typedef struct _Wav_Header_{

    char riff[4];
    unsigned int size;
    char wav[4];
    char fmt[4];
    unsigned int fmt_size;
    uint16_t format;
    uint16_t channels;
    unsigned int wav_sample_rate;
    unsigned int bytes_per_second;
    uint16_t block_size;
    uint16_t bit;
    char data[4];
    unsigned int data_size;

}Wav_Header;

typedef struct _Unit_Dict_{

	char key[unit_byte_max];
    char value[unit_byte_max];

}Unit_Dict;

typedef struct _WoHoSpeech_ASR_{
    FBANK *fb;
    CTC_Endpoint *ctc_endpoint_detector;
    CTC_Prefix_Beam_Search *ctc_decoder;
    CTC_Prefix_Beam_Search_WRT_Timestamp *ctc_decoder_wrt_timestamp;

    int vocab_size;

    int decoding_chunk_size;
    int subsampling;
    int right_context;
    int context;
    int stride;
    int decoding_window;
    int offset;
    int num_decoding_left_chunks;
    
    // subsampling
    int idim;
    int odim;
    Conv2D *sub4_conv_conv2d_1;
    ReLU *sub4_conv_relu_1;
    int sub4_conv_conv2d_1_h_out;
    int sub4_conv_conv2d_1_w_out;
    Conv2D *sub4_conv_conv2d_2;
    ReLU *sub4_conv_relu_2;
    int sub4_conv_conv2d_2_h_out;
    int sub4_conv_conv2d_2_w_out;
    Linear *sub4_out_linear;

    // embedding
    float pos_enc_xscale;
    int pos_max_len;
    float **pos_pe;

    // fead forward macaron param
    int linear_units;

    // multi head attention param
    int head;
    int d_k;
    int d_k_sqrt;

    Mat *q_view;
    Mat *k_view;
    Mat *v_view;

    Mat *q_view_t;
    Mat *k_view_t;
    Mat *v_view_t;

    Mat *k_cache[num_encoder_layer][k_v_cache_max]; 
    Mat *v_cache[num_encoder_layer][k_v_cache_max];

    Mat *q_with_bias_u;
    Mat *q_with_bias_v;
    Mat *q_with_bias_u_t;
    Mat *q_with_bias_v_t;
    
    Mat *att_out;
    Mat *att_out_t;
    Mat *att_out_t_view;

    // convolution module param
    int cnn_module_kernel;
    int lorder;

    Mat *conv_x_t;
    Mat *conv_x_t_pad;
    
    Mat *conv_cache[num_encoder_layer];

    Mat *depthwise_conv_out_t;
    Mat *conformer_conv_module_act_out_t;
    Mat *conv_out;

    // conformer encoder layer
    float ff_scale;
    Mat *conformer_residual;

    /*-------------------------encoder start-------------------------*/
    // feed foward macaron
    LayerNorm *conformer_norm_ff_macaron[num_encoder_layer];
    Linear *conformer_feed_forward_macaron_w_1[num_encoder_layer];
    Linear *conformer_feed_forward_macaron_w_2[num_encoder_layer];
    SiLU *conformer_feed_forward_macaron_act;

    // multi head attention
    LayerNorm *conformer_norm_mha[num_encoder_layer];
    Linear *conformer_self_attn_linear_q[num_encoder_layer];
    Linear *conformer_self_attn_linear_k[num_encoder_layer];
    Linear *conformer_self_attn_linear_v[num_encoder_layer];
    Linear *conformer_self_attn_linear_out[num_encoder_layer];
    Linear *conformer_self_attn_linear_pos[num_encoder_layer];
    Mat *pos_bias_u[num_encoder_layer];
    Mat *pos_bias_v[num_encoder_layer];
    Softmax *conformer_self_attn_softmax;

    // convolution module
    LayerNorm *conformer_norm_conv[num_encoder_layer];
    Conv1D *conformer_conv_module_pointwise_conv1[num_encoder_layer];
    Depthwise_Conv1D *conformer_conv_module_depthwise_conv[num_encoder_layer];
    LayerNorm *conformer_conv_module_norm[num_encoder_layer];
    Conv1D *conformer_conv_module_pointwise_conv2[num_encoder_layer];
    SiLU *conformer_conv_module_act;
    GLU *conformer_conv_module_glu;

    // feed forward
    LayerNorm *conformer_norm_ff[num_encoder_layer];
    Linear *conformer_feed_forward_w_1[num_encoder_layer];
    Linear *conformer_feed_forward_w_2[num_encoder_layer];
    LayerNorm *conformer_norm_final[num_encoder_layer];
    SiLU *conformer_feed_forward_act;

    LayerNorm *conformer_after_norm;
    Linear *ctc_lo;
    Softmax *ctc_lo_softmax;
    /*-------------------------encoder end-------------------------*/

    int decoded_something;
    
    int chunk_xs_idx;
    Mat *chunk_xs;

    Mat *sub4_t;
    Mat *sub4_t_view;

}WoHoSpeech_ASR;

WoHoSpeech_ASR *wohospeech_asr_create(char *model_param_path);
int wohospeech_asr_destroy(WoHoSpeech_ASR *asr);
int wohospeech_asr_process(WoHoSpeech_ASR *asr, float *wave, int num_samples, char *dict_path);

#endif
