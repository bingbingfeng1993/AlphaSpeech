#include "asr_decoder.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
/*
float mean[num_bins] = {
     9.87178848, 9.93892857,  10.23821282, 10.85969755, 11.68652886, 12.25486235, 12.65767214, 12.86137458, 
    12.80736634, 12.56627468, 12.3200896,  12.13880743, 12.31316856, 12.55253322, 12.61227051, 12.56974407, 
    12.38976042, 12.14382391, 12.09285658, 11.79399136, 11.62257881, 11.9263161,  11.81548718, 11.95123278, 
    11.83178659, 11.88789943, 11.79014319, 11.88070614, 11.90004527, 11.97347076, 12.00978756, 12.00880273, 
    12.02619825, 12.10481895, 12.21556501, 12.34402026, 12.45018307, 12.49670684, 12.48650698, 12.35509132, 
    12.39289957, 12.25538435, 12.26496041, 12.2531093,  12.32541739, 12.43361378, 12.54867794, 12.67635195, 
    12.80922355, 12.92931958, 12.96111942, 12.96881932, 12.99593293, 13.04728142, 13.0588875,  13.05738421, 
    12.99919992, 12.93399752, 12.87431586, 12.7165193,  12.48943069, 12.27480516, 12.26162832, 12.2863201, 
    12.31956522, 12.42288126, 12.51477587, 12.57854718, 12.64719056, 12.73765054, 12.80018253, 12.86870044, 
    12.96667803, 13.06476807, 13.15916974, 13.27283123, 13.31080874, 13.23900136, 12.87933944, 11.18311605
};

float istd[num_bins] = {
    0.61225775, 0.49702525, 0.33440483, 0.31502779, 0.29640698, 0.2841113,  0.26972575, 0.25610063, 
    0.24633188, 0.24610597, 0.24733494, 0.24426726, 0.23751373, 0.22987168, 0.22660443, 0.22684263, 
    0.2305914,  0.23420635, 0.23772162, 0.24113962, 0.24404005, 0.24556959, 0.24725965, 0.250551, 
    0.25482134, 0.26024794, 0.26364165, 0.26503725, 0.26484426, 0.26434548, 0.26352706, 0.26364577, 
    0.26410923, 0.26340792, 0.26212377, 0.26147367, 0.26197124, 0.26366223, 0.26592272, 0.26963865, 
    0.27392041, 0.27818804, 0.2831377,  0.28632408, 0.28711987, 0.28649573, 0.28636664, 0.28678983, 
    0.28636514, 0.28561312, 0.28491578, 0.28428843, 0.2840346,  0.28400697, 0.28561499, 0.28727955, 
    0.28968921, 0.29350933, 0.29826633, 0.30572488, 0.31812202, 0.32889478, 0.33287658, 0.33326615, 
    0.33014697, 0.32402359, 0.31902978, 0.31317214, 0.30741485, 0.30371367, 0.30205694, 0.30047676, 
    0.29902091, 0.29823846, 0.29813056, 0.29752516, 0.29778809, 0.30173216, 0.30954893, 0.32904991
};
*/

float mean[num_bins] = {
     9.94003899, 10.32859269, 11.09406287, 11.81050998, 12.4632706,  12.91553895, 13.16243044, 13.17006559, 
    13.15276202, 13.08630868, 13.25131352, 13.41853687, 13.69534652, 13.88551681, 13.9364746,  13.98411869, 
    13.97366779, 13.88587693, 13.92851342, 13.63884021, 13.40207062, 13.53135239, 13.2871092,  13.38230549, 
    13.28460169, 13.36331099, 13.26356354, 13.34688179, 13.32897209, 13.36872317, 13.42492716, 13.4822041, 
    13.59181707, 13.81764818, 13.95992393, 14.01732684, 14.13463423, 14.2432113,  14.35749695, 14.33047161, 
    14.44408888, 14.39872259, 14.47546975, 14.46605475, 14.52020916, 14.60195355, 14.68868034, 14.78442645, 
    14.85631421, 14.91115922, 14.91947312, 14.93149936, 14.97354899, 14.99508953, 14.96615096, 14.96491795, 
    14.96110038, 14.95399133, 14.95793635, 14.94514689, 14.96129452, 14.90291647, 14.78269847, 14.69410438, 
    14.5980026,  14.50242395, 14.40444599, 14.30578779, 14.22576843, 14.17060923, 14.10755258, 14.08380834, 
    14.08063691, 14.07443928, 14.04258121, 13.98833788, 13.94464178, 13.83592837, 13.74766328, 13.64538758
};

float istd[num_bins] = {
    0.27379465, 0.25615304, 0.23137241, 0.22185596, 0.21117136, 0.20323631, 0.19793547, 0.19604027, 
    0.19893922, 0.20306119, 0.20391606, 0.20214536, 0.20007525, 0.19937448, 0.20005627, 0.20153953, 
    0.20357805, 0.20552361, 0.20772628, 0.21091971, 0.21388068, 0.21669553, 0.22005216, 0.22316562, 
    0.22613023, 0.22910066, 0.23125481, 0.23292176, 0.23346968, 0.23407341, 0.23491962, 0.23583043, 
    0.23940161, 0.25058894, 0.24932353, 0.23759215, 0.23687175, 0.23718054, 0.23773344, 0.23852041, 
    0.23963421, 0.24080325, 0.24240329, 0.24239222, 0.24188307, 0.24101476, 0.23941912, 0.2378165, 
    0.23723269, 0.23782822, 0.2394008,  0.23923603, 0.2401608,  0.24000996, 0.24065761, 0.24148788, 
    0.24183238, 0.24182334, 0.24189355, 0.24218859, 0.2446124,  0.24806756, 0.24375489, 0.24409566, 
    0.24481161, 0.24567932, 0.24696875, 0.24715897, 0.24732685, 0.2486059,  0.24879275, 0.25009108, 
    0.2509817,  0.25143804, 0.25227388, 0.25411096, 0.25560143, 0.25390268, 0.25340287, 0.25329341
};

static void global_cmvn(float *in, float *mean, float *istd, float len) {
    for (int i = 0; i < len; i ++) {
        in[i] = (in[i] - mean[i]) * istd[i];
    }
}

float fill_zeros[num_bins] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

WoHoSpeech_ASR *wohospeech_asr_create(char *model_param_path) {
    
    WoHoSpeech_ASR *asr = (WoHoSpeech_ASR *)malloc(sizeof(WoHoSpeech_ASR));

    asr->fb = fbank_create(num_bins, sample_rate, frame_length, frame_shift, true, true);

    // asr->vocab_size = 4233;
    asr->vocab_size = 5002;
    
    asr->decoding_chunk_size = 16;  // 4, 8, 16, ...
    asr->subsampling = 4;
    asr->right_context = 6;
    asr->context = asr->right_context + 1;
    asr->stride = asr->subsampling * asr->decoding_chunk_size;
    asr->decoding_window = (asr->decoding_chunk_size - 1) * asr->subsampling + asr->context;
    asr->offset = 0;
    asr->num_decoding_left_chunks = -1;

    asr->decoded_something = 0;
    
    asr->chunk_xs_idx = 0;
    asr->chunk_xs = Mat_2D_create(num_bins, asr->decoding_window, 4, 1);
    // printf("%d, %d\n", asr->chunk_xs->w, asr->chunk_xs->h);  // 80, 67

    char path_tmp_name[256];

    // subsampling
    asr->idim = 80;
    asr->odim = 256;

    asr->sub4_conv_conv2d_1 = Conv2D_create(1, asr->odim, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0.0, true);
    sprintf(path_tmp_name, "%s/%s", model_param_path, "Subsampling4_Conv_Conv2D_1");
    Conv2D_load_variables(asr->sub4_conv_conv2d_1, path_tmp_name);

    asr->sub4_conv_relu_1 = ReLU_create();

    asr->sub4_conv_conv2d_2 = Conv2D_create(asr->odim, asr->odim, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0.0, true);
    sprintf(path_tmp_name, "%s/%s", model_param_path, "Subsampling4_Conv_Conv2D_2");
    Conv2D_load_variables(asr->sub4_conv_conv2d_2, path_tmp_name);

    asr->sub4_conv_relu_2 = ReLU_create();

    asr->sub4_conv_conv2d_1_h_out = (int)((asr->decoding_window + 2 * 0 - 1 * (3 - 1) - 1) / 2 + 1);
    asr->sub4_conv_conv2d_1_w_out = (int)((asr->idim + 2 * 0 - 1 * (3 - 1) - 1) / 2 + 1);
    asr->sub4_conv_conv2d_2_h_out = (int)((asr->sub4_conv_conv2d_1_h_out + 2 * 0 - 1 * (3 - 1) - 1) / 2 + 1);
    asr->sub4_conv_conv2d_2_w_out = (int)((asr->sub4_conv_conv2d_1_w_out + 2 * 0 - 1 * (3 - 1) - 1) / 2 + 1);

    asr->sub4_t = Mat_3D_create(asr->sub4_conv_conv2d_2_w_out, asr->odim, asr->sub4_conv_conv2d_2_h_out, 4, 1);  // (w, c, h)
    asr->sub4_t_view = Mat_2D_create(asr->odim * asr->sub4_conv_conv2d_2_w_out, asr->sub4_conv_conv2d_2_h_out, 4, 1);

    int sub4_out_linear_tmp_param = (int)(((int)((asr->idim - 1) / 2) - 1) / 2);
    asr->sub4_out_linear = Linear_create(asr->odim * sub4_out_linear_tmp_param, asr->odim, true);
    sprintf(path_tmp_name, "%s/%s", model_param_path, "Subsampling4_Out_Linear");
    Linear_load_variables(asr->sub4_out_linear, path_tmp_name);

    // embedding
    asr->pos_enc_xscale = (float)sqrt(asr->odim);
    asr->pos_max_len = 5000;

    asr->pos_pe = (float **)malloc(sizeof(float *) * asr->pos_max_len);
    for (int i = 0; i < asr->pos_max_len; i ++) {
        asr->pos_pe[i] = (float *)malloc(sizeof(float) * asr->odim);
    }

    for (int i = 0; i < asr->pos_max_len; i ++) {
        for (int j = 0; j < asr->odim; j += 2) {
            float div_term = exp(-j * (log(10000.0) / asr->odim));
            asr->pos_pe[i][j] = sin(i * div_term);
            asr->pos_pe[i][j + 1] = cos(i * div_term);
            // printf("%f ", div_term);
            // printf("%f, %f ", asr->pos_pe[i][j], asr->pos_pe[i][j + 1]);
        }
        // printf("\n");
    }

    // fead forward macaron param
    asr->linear_units = 2048;
    asr->conformer_feed_forward_macaron_act = SiLU_create();
 
    // multi head attention parm
    asr->head = 4;
    asr->d_k = asr->odim / asr->head;
    asr->d_k_sqrt = (int)sqrt(asr->d_k);
    asr->conformer_self_attn_softmax = Softmax_create(2);

    asr->q_view = Mat_3D_create(asr->d_k, asr->head, asr->sub4_conv_conv2d_2_h_out, 4, 1);
    asr->k_view = Mat_3D_create(asr->d_k, asr->head, asr->sub4_conv_conv2d_2_h_out, 4, 1);
    asr->v_view = Mat_3D_create(asr->d_k, asr->head, asr->sub4_conv_conv2d_2_h_out, 4, 1);
    asr->q_view_t = Mat_3D_create(asr->d_k, asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
    asr->k_view_t = Mat_3D_create(asr->d_k, asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
    asr->v_view_t = Mat_3D_create(asr->d_k, asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
    // q = self.linear_q(query).view(n_batch, -1, self.h, self.d_k)
    // k = self.linear_k(key).view(n_batch, -1, self.h, self.d_k)
    // v = self.linear_v(value).view(n_batch, -1, self.h, self.d_k)
    // q = q.transpose(1, 2)  # (batch, head, time1, d_k)
    // k = k.transpose(1, 2)  # (batch, head, time2, d_k)
    // v = v.transpose(1, 2)  # (batch, head, time2, d_k)

    for (int i = 0; i < num_encoder_layer; i ++) {
        for (int j = 0; j < k_v_cache_max; j ++) {
            asr->k_cache[i][j] = Mat_3D_create(asr->d_k, (j + 1) * asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
            asr->v_cache[i][j] = Mat_3D_create(asr->d_k, (j + 1) * asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
        }
    }

    asr->q_with_bias_u = Mat_3D_create(asr->d_k, asr->head, asr->sub4_conv_conv2d_2_h_out, 4, 1);
    asr->q_with_bias_v = Mat_3D_create(asr->d_k, asr->head, asr->sub4_conv_conv2d_2_h_out, 4, 1);
    asr->q_with_bias_u_t = Mat_3D_create(asr->d_k, asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
    asr->q_with_bias_v_t = Mat_3D_create(asr->d_k, asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
    // q_with_bias_u = (q + self.pos_bias_u).transpose(1, 2)
    // q_with_bias_v = (q + self.pos_bias_v).transpose(1, 2)

    asr->att_out = Mat_3D_create(asr->d_k, asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
    asr->att_out_t = Mat_3D_create(asr->d_k, asr->head, asr->sub4_conv_conv2d_2_h_out, 4, 1);
    asr->att_out_t_view = Mat_2D_create(asr->odim, asr->sub4_conv_conv2d_2_h_out, 4, 1);
    // x = (x.transpose(1, 2).contiguous().view(n_batch, -1, self.h * self.d_k)

    // convolution module param
    asr->cnn_module_kernel = 15;
    asr->conformer_conv_module_act = SiLU_create();
    asr->lorder = asr->cnn_module_kernel - 1;
    asr->conformer_conv_module_glu = GLU_create(0);

    asr->conv_x_t = Mat_2D_create(asr->sub4_conv_conv2d_2_h_out, asr->odim, 4, 1);
    // x = x.transpose(1, 2)  # (#batch, channels, time)
    asr->conv_x_t_pad = Mat_2D_create(asr->sub4_conv_conv2d_2_h_out + asr->lorder, asr->odim, 4, 1);
    // x = nn.functional.pad(x, (self.lorder, 0), 'constant', 0.0)

    for (int i = 0; i < num_encoder_layer; i ++) {
        asr->conv_cache[i] = Mat_2D_create(asr->lorder, asr->odim, 4, 1);
    }
    
    asr->depthwise_conv_out_t = Mat_2D_create(asr->odim, asr->sub4_conv_conv2d_2_h_out, 4, 1);
    asr->conformer_conv_module_act_out_t = Mat_2D_create(asr->sub4_conv_conv2d_2_h_out, asr->odim, 4, 1);
    asr->conv_out = Mat_2D_create(asr->odim, asr->sub4_conv_conv2d_2_h_out, 4, 1);

    // fead forward param
    asr->conformer_feed_forward_act = SiLU_create();

    // conformer encoder layer
    asr->ff_scale = 0.5;
    asr->conformer_residual = Mat_2D_create(asr->odim, asr->sub4_conv_conv2d_2_h_out, 4, 1);

    for (int i = 0; i < num_encoder_layer; i ++) {
        // feed forward macaron
        asr->conformer_norm_ff_macaron[i] = LayerNorm_create(asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_norm_ff_macaron");
        LayerNorm_load_variables(asr->conformer_norm_ff_macaron[i], path_tmp_name);

        asr->conformer_feed_forward_macaron_w_1[i] = Linear_create(asr->odim, asr->linear_units, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_feed_forward_macaron_w_1");
        Linear_load_variables(asr->conformer_feed_forward_macaron_w_1[i], path_tmp_name);

        asr->conformer_feed_forward_macaron_w_2[i] = Linear_create(asr->linear_units, asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_feed_forward_macaron_w_2");
        Linear_load_variables(asr->conformer_feed_forward_macaron_w_2[i], path_tmp_name);

        // multi head attention 
        asr->conformer_norm_mha[i] = LayerNorm_create(asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_norm_mha");
        LayerNorm_load_variables(asr->conformer_norm_mha[i], path_tmp_name);

        asr->conformer_self_attn_linear_q[i] = Linear_create(asr->odim, asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_self_attn_linear_q");
        Linear_load_variables(asr->conformer_self_attn_linear_q[i], path_tmp_name);

        asr->conformer_self_attn_linear_k[i] = Linear_create(asr->odim, asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_self_attn_linear_k");
        Linear_load_variables(asr->conformer_self_attn_linear_k[i], path_tmp_name);

        asr->conformer_self_attn_linear_v[i] = Linear_create(asr->odim, asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_self_attn_linear_v");
        Linear_load_variables(asr->conformer_self_attn_linear_v[i], path_tmp_name);

        asr->conformer_self_attn_linear_out[i] = Linear_create(asr->odim, asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_self_attn_linear_out");
        Linear_load_variables(asr->conformer_self_attn_linear_out[i], path_tmp_name);

        asr->conformer_self_attn_linear_pos[i] = Linear_create(asr->odim, asr->odim, false);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_self_attn_linear_pos");
        Linear_load_variables(asr->conformer_self_attn_linear_pos[i], path_tmp_name);

        asr->pos_bias_u[i] = Mat_2D_create(asr->d_k, asr->head, 4, 1);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_self_attn_pos_bias_u");
        Parameter_load_variables(path_tmp_name, asr->pos_bias_u[i]);

        asr->pos_bias_v[i] = Mat_2D_create(asr->d_k, asr->head, 4, 1);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_self_attn_pos_bias_v");
        Parameter_load_variables(path_tmp_name, asr->pos_bias_v[i]);
        
        // convolution module
        asr->conformer_norm_conv[i] = LayerNorm_create(asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_norm_conv");
        LayerNorm_load_variables(asr->conformer_norm_conv[i], path_tmp_name);

        asr->conformer_conv_module_pointwise_conv1[i] = Conv1D_create(asr->odim, asr->odim * 2, 1, 1, 1, 0, 0, 0.0, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_conv_module_pointwise_conv1");
        Conv1D_load_variables(asr->conformer_conv_module_pointwise_conv1[i], path_tmp_name);
        
        asr->conformer_conv_module_depthwise_conv[i] = Depthwise_Conv1D_create(asr->odim, asr->odim, asr->cnn_module_kernel, 1, 1, asr->odim, 0, 0, 0.0, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_conv_module_depthwise_conv");
        Depthwise_Conv1D_load_variables(asr->conformer_conv_module_depthwise_conv[i], path_tmp_name);

        asr->conformer_conv_module_norm[i] = LayerNorm_create(asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_conv_module_norm");
        LayerNorm_load_variables(asr->conformer_conv_module_norm[i], path_tmp_name);

        asr->conformer_conv_module_pointwise_conv2[i] = Conv1D_create(asr->odim, asr->odim, 1, 1, 1, 0, 0, 0.0, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_conv_module_pointwise_conv2");
        Conv1D_load_variables(asr->conformer_conv_module_pointwise_conv2[i], path_tmp_name);

        // fead forward
        asr->conformer_norm_ff[i] = LayerNorm_create(asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_norm_ff");
        LayerNorm_load_variables(asr->conformer_norm_ff[i], path_tmp_name);

        asr->conformer_feed_forward_w_1[i] = Linear_create(asr->odim, asr->linear_units, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_feed_forward_w_1");
        Linear_load_variables(asr->conformer_feed_forward_w_1[i], path_tmp_name);

        asr->conformer_feed_forward_w_2[i] = Linear_create(asr->linear_units, asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_feed_forward_w_2");
        Linear_load_variables(asr->conformer_feed_forward_w_2[i], path_tmp_name);

        asr->conformer_norm_final[i] = LayerNorm_create(asr->odim, true);
        sprintf(path_tmp_name, "%s/%s%d%s", model_param_path, "Conformer", i, "_norm_final");
        LayerNorm_load_variables(asr->conformer_norm_final[i], path_tmp_name);
    }

    asr->conformer_after_norm = LayerNorm_create(asr->odim, true);
    sprintf(path_tmp_name, "%s/%s", model_param_path, "Conformer_after_norm");
    LayerNorm_load_variables(asr->conformer_after_norm, path_tmp_name);

    asr->ctc_lo = Linear_create(asr->odim, asr->vocab_size, true);
    sprintf(path_tmp_name, "%s/%s", model_param_path, "CTC_ctc_lo");
    Linear_load_variables(asr->ctc_lo, path_tmp_name);

    asr->ctc_lo_softmax = Softmax_create(1);

    return asr;
}

int wohospeech_asr_process(WoHoSpeech_ASR *asr, float *wave, int num_samples, char *dict_path) {

    int num_frames = 1 + ((num_samples - frame_length) / frame_shift);
    // printf("%d\n", num_frames);

    float **feat = (float **)malloc(sizeof(float *) * num_frames);
    for (int i = 0; i < num_frames; i ++) {
        feat[i] = (float *)malloc(sizeof(float) * num_bins);
    }

    fbank_process(asr->fb, wave, num_samples, feat);

    // load dict
    FILE *dict_fp = fopen(dict_path, "r");

    Unit_Dict char_dict[asr->vocab_size];

    for (int i = 0; fscanf(dict_fp, "%s %s", char_dict[i].value, char_dict[i].key) != EOF; i ++) {}

    fclose(dict_fp);

    int cont_time_step = 0;

    int res_msg_char_num = 1;
    char *res_msg = (char *)malloc(sizeof(char) * res_msg_char_num);
    res_msg[0] = '\0';

    float **ctc_endpoint_probs = NULL;
    int ctc_endpoint_probs_size = 0;

    int frame_shift_in_ms = asr->subsampling * frame_shift * 1000 / sample_rate;
    int time_stamp_gap = 100;

    // forward_chunk_by_chunk
    for (int cur = 0; cur < num_frames - asr->context + 1; cur += asr->stride) {
        #if true
        struct timeval time_start, time_end;
        gettimeofday(&time_start, NULL);
        #endif

        int end = MIN(cur + asr->decoding_window, num_frames);
        // printf("%d, %d\n", cur, end);

        for (int i = 0; i < asr->decoding_window; i ++) {  // in-place
            float *chunk_xs_row = (float *)Mat_row(asr->chunk_xs, i);
            if (i < end - cur) {
                memcpy((void *)chunk_xs_row, feat[cur + i], sizeof(float) * num_bins);
            } else {
                memcpy((void *)chunk_xs_row, fill_zeros, sizeof(float) * num_bins);
            }
            global_cmvn(chunk_xs_row, mean, istd, num_bins);
        }
        // Mat_2D_float_printf(asr->chunk_xs);

        // subsampling
        Conv2D_process(asr->sub4_conv_conv2d_1, asr->chunk_xs);
        Mat *sub4_conv_conv2d_1_out = Conv2D_get_output(asr->sub4_conv_conv2d_1);
        ReLU_process_inplace(asr->sub4_conv_relu_1, sub4_conv_conv2d_1_out);
        // printf("%d, %d, %d\n", sub4_conv_conv2d_1_out->w, sub4_conv_conv2d_1_out->h, sub4_conv_conv2d_1_out->c);

        Conv2D_process(asr->sub4_conv_conv2d_2, sub4_conv_conv2d_1_out);
        Mat *sub4_conv_conv2d_2_out = Conv2D_get_output(asr->sub4_conv_conv2d_2);
        ReLU_process_inplace(asr->sub4_conv_relu_2, sub4_conv_conv2d_2_out);
        // printf("%d, %d, %d\n", sub4_conv_conv2d_2_out->w, sub4_conv_conv2d_2_out->h, sub4_conv_conv2d_2_out->c);

        Permute_process(sub4_conv_conv2d_2_out, asr->sub4_t, 2);
        // printf("%d, %d, %d\n", asr->sub4_t->w, asr->sub4_t->c, asr->sub4_t->h);
        // Mat_3D_Channel_float_printf(asr->sub4_t, 4);

        memcpy(asr->sub4_t_view->data, asr->sub4_t->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_w_out * asr->sub4_conv_conv2d_2_h_out);
        // Mat_2D_float_printf(asr->sub4_t_view);

        Linear_process(asr->sub4_out_linear, asr->sub4_t_view);
        Mat *sub4_out_linear_out = Linear_get_output(asr->sub4_out_linear);
        // printf("%d, %d, %d\n", sub4_out_linear_out->w, sub4_out_linear_out->h, sub4_out_linear_out->c);
        // Mat_2D_float_printf(asr->sub4_out_linear_out);

        // embedding
        for (int i = 0; i < sub4_out_linear_out->h; i ++) {
            for (int j = 0; j < sub4_out_linear_out->w; j ++) {
                ((float *)sub4_out_linear_out->data)[i * sub4_out_linear_out->w + j] *= asr->pos_enc_xscale;
            }
        }
        // printf("%d, %d, %d\n", sub4_out_linear_out->w, sub4_out_linear_out->h, sub4_out_linear_out->c);
        // Mat_2D_float_printf(sub4_out_linear_out);

        // ****** pos_emb = self.embed.position_encoding(offset - cache_size, xs.size(1)) ******
        asr->offset += asr->sub4_conv_conv2d_2_h_out;
        // printf("%d\n", asr->offset);

        Mat *pos_emb = Mat_2D_create(asr->odim, asr->offset, 4, 1);
        for (int i = 0; i < pos_emb->h; i ++) {
            for (int j = 0; j < pos_emb->w; j ++) {
                ((float *)pos_emb->data)[i * pos_emb->w + j] = asr->pos_pe[i][j];
            }
        }
        // Mat_2D_float_printf(pos_emb);
        // ****** pos_emb = self.embed.position_encoding(offset - cache_size, xs.size(1)) ******

        // multi head attention 

        Mat *conformer_self_attn_linear_pos_out = Mat_2D_create(asr->odim, asr->offset, 4, 1);
        Mat *conformer_self_attn_linear_pos_out_view = Mat_3D_create(asr->d_k, asr->head, asr->offset, 4, 1);
        Mat *conformer_self_attn_linear_pos_out_view_t = Mat_3D_create(asr->d_k, asr->offset, asr->head, 4, 1);

        Mat *matrix_ac = Mat_3D_create(asr->offset, asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
        Mat *matrix_bd = Mat_3D_create(asr->offset, asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);

        for (int layer = 0; layer < num_encoder_layer; layer ++) {
            // 1.feed forward macaron
            // Mat_2D_float_printf(sub4_out_linear_out);            
            memcpy(asr->conformer_residual->data, sub4_out_linear_out->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            LayerNorm_process_inplace(asr->conformer_norm_ff_macaron[layer], sub4_out_linear_out);
            // Mat_2D_float_printf(sub4_out_linear_out);

            Linear_process(asr->conformer_feed_forward_macaron_w_1[layer], sub4_out_linear_out);
            Mat *conformer_feed_forward_macaron_w_1_out = Linear_get_output(asr->conformer_feed_forward_macaron_w_1[layer]);
            // Mat_2D_float_printf(conformer_feed_forward_macaron_w_1_out);

            SiLU_process_inplace(asr->conformer_feed_forward_macaron_act, conformer_feed_forward_macaron_w_1_out);
            // Mat_2D_float_printf(conformer_feed_forward_macaron_w_1_out);

            Linear_process(asr->conformer_feed_forward_macaron_w_2[layer], conformer_feed_forward_macaron_w_1_out);
            Mat *conformer_feed_forward_macaron_w_2_out = Linear_get_output(asr->conformer_feed_forward_macaron_w_2[layer]);
            // Mat_2D_float_printf(conformer_feed_forward_macaron_w_2_out);

            for (int i = 0; i < conformer_feed_forward_macaron_w_2_out->h; i ++) {
                for (int j = 0; j < conformer_feed_forward_macaron_w_2_out->w; j ++) {
                    ((float *)conformer_feed_forward_macaron_w_2_out->data)[i * conformer_feed_forward_macaron_w_2_out->w + j] = \
                    ((float *)asr->conformer_residual->data)[i * conformer_feed_forward_macaron_w_2_out->w + j] + \
                    ((float *)conformer_feed_forward_macaron_w_2_out->data)[i * conformer_feed_forward_macaron_w_2_out->w + j] * asr->ff_scale;
                }
            }
            // Mat_2D_float_printf(conformer_feed_forward_macaron_w_2_out);

            // 2.multi head attention
            memcpy(asr->conformer_residual->data, conformer_feed_forward_macaron_w_2_out->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            LayerNorm_process_inplace(asr->conformer_norm_mha[layer], conformer_feed_forward_macaron_w_2_out);
            // printf("%d, %d, %d\n", conformer_feed_forward_macaron_w_2_out->w, conformer_feed_forward_macaron_w_2_out->h, conformer_feed_forward_macaron_w_2_out->c);
            // Mat_2D_float_printf(conformer_feed_forward_macaron_w_2_out);

            Linear_process(asr->conformer_self_attn_linear_q[layer], conformer_feed_forward_macaron_w_2_out);
            Mat *q = Linear_get_output(asr->conformer_self_attn_linear_q[layer]);
            // Mat_2D_float_printf(q);

            Linear_process(asr->conformer_self_attn_linear_k[layer], conformer_feed_forward_macaron_w_2_out);
            Mat *k = Linear_get_output(asr->conformer_self_attn_linear_k[layer]);
            // Mat_2D_float_printf(k);

            Linear_process(asr->conformer_self_attn_linear_v[layer], conformer_feed_forward_macaron_w_2_out);
            Mat *v = Linear_get_output(asr->conformer_self_attn_linear_v[layer]);
            // Mat_2D_float_printf(v);

            memcpy(asr->q_view->data, q->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            // Mat_3D_Channel_float_printf(asr->q_view, 0);

            memcpy(asr->k_view->data, k->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            // Mat_3D_Channel_float_printf(asr->k_view, 0);

            memcpy(asr->v_view->data, v->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            // Mat_3D_Channel_float_printf(asr->v_view, 0);

            Permute_process(asr->q_view, asr->q_view_t, 2);
            Permute_process(asr->k_view, asr->k_view_t, 2);
            Permute_process(asr->v_view, asr->v_view_t, 2);
            // Mat_3D_Channel_float_printf(asr->q_view_t, 3);
            // Mat_3D_Channel_float_printf(asr->k_view_t, 3);
            // Mat_3D_Channel_float_printf(asr->v_view_t, 3);

            Linear_process_use_extern_output(asr->conformer_self_attn_linear_pos[layer], pos_emb, conformer_self_attn_linear_pos_out);
            // p = self.linear_pos(pos_emb).view(n_batch_pos, -1, self.h, self.d_k)
            // Mat_2D_float_printf(conformer_self_attn_linear_pos_out);
            memcpy(conformer_self_attn_linear_pos_out_view->data, conformer_self_attn_linear_pos_out->data, sizeof(float) * asr->d_k * asr->head * asr->offset);
            // Mat_3D_Channel_float_printf(conformer_self_attn_linear_pos_out_view, 3);
            Permute_process(conformer_self_attn_linear_pos_out_view, conformer_self_attn_linear_pos_out_view_t, 2);
            // p = p.transpose(1, 2)
            // Mat_3D_Channel_float_printf(conformer_self_attn_linear_pos_out_view_t, 3);

            for (int i = 0; i < asr->q_view->c; i ++) {
                float *in_channel_data = (float *)Mat_channel(asr->q_view, i);
                float *out_channel_data = (float *)Mat_channel(asr->q_with_bias_u, i);
                for (int j = 0; j < asr->q_view->h; j ++) {
                    for (int k = 0; k < asr->q_view->w; k ++) {
                        out_channel_data[j * asr->q_view->w + k] = in_channel_data[j * asr->q_view->w + k] + ((float *)asr->pos_bias_u[layer]->data)[j * asr->q_view->w + k];
                    }
                }
            }

            Permute_process(asr->q_with_bias_u, asr->q_with_bias_u_t, 2);
            // q_with_bias_u = (q + self.pos_bias_u).transpose(1, 2)
            // Mat_3D_Channel_float_printf(asr->q_with_bias_u_t, 3);

            for (int i = 0; i < asr->q_view->c; i ++) {
                float *in_channel_data = (float *)Mat_channel(asr->q_view, i);
                float *out_channel_data = (float *)Mat_channel(asr->q_with_bias_v, i);
                for (int j = 0; j < asr->q_view->h; j ++) {
                    for (int k = 0; k < asr->q_view->w; k ++) {
                        out_channel_data[j * asr->q_view->w + k] = in_channel_data[j * asr->q_view->w + k] + ((float *)asr->pos_bias_v[layer]->data)[j * asr->q_view->w + k];
                    }
                }
            }
            Permute_process(asr->q_with_bias_v, asr->q_with_bias_v_t, 2);
            // q_with_bias_v = (q + self.pos_bias_v).transpose(1, 2)
            // Mat_3D_Channel_float_printf(asr->q_with_bias_v_t, 3);

            if (asr->chunk_xs_idx == 0) {
                memcpy(asr->k_cache[layer][asr->chunk_xs_idx]->data, asr->k_view_t->data, sizeof(float) * asr->d_k * asr->offset * asr->head);
                memcpy(asr->v_cache[layer][asr->chunk_xs_idx]->data, asr->v_view_t->data, sizeof(float) * asr->d_k * asr->offset * asr->head);
            } else {
                // copy
                for (int i = 0; i < asr->k_cache[layer][asr->chunk_xs_idx]->c; i ++) {
                    float *k_cache_channel_data = (float *)Mat_channel(asr->k_cache[layer][asr->chunk_xs_idx], i);
                    float *k_cache_channel_pre_data = (float *)Mat_channel(asr->k_cache[layer][asr->chunk_xs_idx - 1], i);
                    float *k_cur_data = (float *)Mat_channel(asr->k_view_t, i);

                    int k_cache_pre_len = asr->k_cache[layer][asr->chunk_xs_idx - 1]->w * asr->k_cache[layer][asr->chunk_xs_idx - 1]->h;
                    memcpy(k_cache_channel_data, k_cache_channel_pre_data, sizeof(float) * k_cache_pre_len);
                    memcpy(k_cache_channel_data + k_cache_pre_len, k_cur_data, sizeof(float) * asr->k_view_t->w * asr->k_view_t->h);
                }

                for (int i = 0; i < asr->v_cache[layer][asr->chunk_xs_idx]->c; i ++) {
                    float *v_cache_channel_data = (float *)Mat_channel(asr->v_cache[layer][asr->chunk_xs_idx], i);
                    float *v_cache_channel_pre_data = (float *)Mat_channel(asr->v_cache[layer][asr->chunk_xs_idx - 1], i);
                    float *v_cur_data = (float *)Mat_channel(asr->v_view_t, i);

                    int v_cache_pre_len = asr->v_cache[layer][asr->chunk_xs_idx - 1]->w * asr->v_cache[layer][asr->chunk_xs_idx - 1]->h;
                    memcpy(v_cache_channel_data, v_cache_channel_pre_data, sizeof(float) * v_cache_pre_len);
                    memcpy(v_cache_channel_data + v_cache_pre_len, v_cur_data, sizeof(float) * asr->v_view_t->w * asr->v_view_t->h);
                }
            }

            for (int i = 0; i < asr->head; i ++) {
                float *matrix_ac_channel_data = (float *)Mat_channel(matrix_ac, i);
                float *k_cache_channel_data = (float *)Mat_channel(asr->k_cache[layer][asr->chunk_xs_idx], i);
                float *q_with_bias_u_t_channel_data = (float *)Mat_channel(asr->q_with_bias_u_t, i);
                for (int j = 0; j < asr->sub4_conv_conv2d_2_h_out; j ++) {              //16
                    for (int k = 0; k < asr->offset; k ++) {                            //32
                        matrix_ac_channel_data[j * asr->offset + k] = 0.0f;
                        for (int l = 0; l < asr->d_k; l ++) {                           //64
                            matrix_ac_channel_data[j * asr->offset + k] += k_cache_channel_data[k * asr->d_k + l] * q_with_bias_u_t_channel_data[j * asr->d_k + l];
                        }
                    }
                }
            }
            // matrix_ac = torch.matmul(q_with_bias_u, k.transpose(-2, -1))
            // Mat_3D_Channel_float_printf(matrix_ac, 3);

            for (int i = 0; i < asr->head; i ++) {
                float *matrix_bd_channel_data = (float *)Mat_channel(matrix_bd, i);
                float *p_channel_data = (float *)Mat_channel(conformer_self_attn_linear_pos_out_view_t, i);
                float *q_with_bias_v_t_channel_data = (float *)Mat_channel(asr->q_with_bias_v_t, i);
                for (int j = 0; j < asr->sub4_conv_conv2d_2_h_out; j ++) {              //16
                    for (int k = 0; k < asr->offset; k ++) {                            //32
                        matrix_bd_channel_data[j * asr->offset + k] = 0.0f;
                        for (int l = 0; l < asr->d_k; l ++) {                           //64
                            matrix_bd_channel_data[j * asr->offset + k] += p_channel_data[k * asr->d_k + l] * q_with_bias_v_t_channel_data[j * asr->d_k + l];
                        }
                    }
                }
            }
            // matrix_bd = torch.matmul(q_with_bias_v, p.transpose(-2, -1))
            // Mat_3D_Channel_float_printf(matrix_bd, 3);

            // scores
            for (int i = 0; i < asr->head; i ++) {
                float *matrix_bd_channel_data = (float *)Mat_channel(matrix_bd, i);
                float *matrix_ac_channel_data = (float *)Mat_channel(matrix_ac, i);
                for (int j = 0; j < asr->sub4_conv_conv2d_2_h_out; j ++) {              //16
                    for (int k = 0; k < asr->offset; k ++) {                            //32
                        matrix_bd_channel_data[j*asr->offset+k] = (matrix_bd_channel_data[j * asr->offset + k] + matrix_ac_channel_data[j * asr->offset + k]) / asr->d_k_sqrt;
                    }
                }
            }
            // scores = (matrix_ac + matrix_bd) / math.sqrt(self.d_k)  # (batch, head, time1, time2)
            // Mat_3D_Channel_float_printf(matrix_bd, 3);

            Softmax_process_inplace(asr->conformer_self_attn_softmax, matrix_bd);
            // attn = torch.softmax(scores, dim=-1).masked_fill(mask, 0.0)  # (batch, head, time1, time2)
            // Mat_3D_Channel_float_printf(matrix_bd, 3);

            for (int i = 0; i < asr->head; i ++) {
                float *att_out_channel_data = (float *)Mat_channel(asr->att_out, i);
                float *matrix_bd_channel_data = (float *)Mat_channel(matrix_bd, i);
                float *v_channel_data = (float *)Mat_channel(asr->v_cache[layer][asr->chunk_xs_idx], i);
                for (int j = 0; j < asr->sub4_conv_conv2d_2_h_out; j ++) {              //16
                    for (int k = 0; k < asr->d_k; k ++) {                               //64
                        att_out_channel_data[j * asr->d_k + k] = 0.0f;
                        for (int l = 0; l < asr->offset; l ++) {                        //32
                            att_out_channel_data[j * asr->d_k + k] += matrix_bd_channel_data[j * asr->offset + l] * v_channel_data[l * asr->d_k + k];
                        }
                    }
                }
            }
            // x = torch.matmul(p_attn, value)  # (batch, head, time1, d_k)
            // Mat_3D_Channel_float_printf(asr->att_out, 3);

            Permute_process(asr->att_out, asr->att_out_t, 2);
            // Mat_3D_Channel_float_printf(asr->att_out_t, 0);
            memcpy(asr->att_out_t_view->data, asr->att_out_t->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            // x = (x.transpose(1, 2).contiguous().view(n_batch, -1, self.h * self.d_k)
            // Mat_2D_float_printf(asr->att_out_t_view);

            Linear_process(asr->conformer_self_attn_linear_out[layer], asr->att_out_t_view);
            Mat *conformer_self_attn_linear_out_out = Linear_get_output(asr->conformer_self_attn_linear_out[layer]);
            // Mat_2D_float_printf(conformer_self_attn_linear_out_out);

            for (int i = 0; i < conformer_self_attn_linear_out_out->h; i ++) {
                for (int j = 0; j < conformer_self_attn_linear_out_out->w; j ++) {
                    ((float *)conformer_self_attn_linear_out_out->data)[i * conformer_self_attn_linear_out_out->w + j] = \
                    ((float *)asr->conformer_residual->data)[i * conformer_self_attn_linear_out_out->w + j] + \
                    ((float *)conformer_self_attn_linear_out_out->data)[i * conformer_self_attn_linear_out_out->w + j];
                }
            }
            // x = residual + self.dropout(x_att)
            // Mat_2D_float_printf(conformer_self_attn_linear_out_out);


            // 3.conv module
            memcpy(asr->conformer_residual->data, conformer_self_attn_linear_out_out->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            LayerNorm_process_inplace(asr->conformer_norm_conv[layer], conformer_self_attn_linear_out_out);
            // Mat_2D_float_printf(conformer_self_attn_linear_out_out);

            Permute_process(conformer_self_attn_linear_out_out, asr->conv_x_t, 1);
            // x = x.transpose(1, 2)  # (#batch, channels, time)

            Mat_2D_float_fill_zeros(asr->conv_x_t_pad);
            float *conv_x_t_data = (float *)asr->conv_x_t->data;
            float *conv_x_t_pad_data = (float *)asr->conv_x_t_pad->data;
            float *conv_cache_data = (float *)asr->conv_cache[layer]->data;

            if (asr->offset == 0) {
                for (int i = 0; i < asr->conv_x_t_pad->h; i ++) {
                    for (int j = 0; j < asr->sub4_conv_conv2d_2_h_out; j ++) {
                        conv_x_t_pad_data[i * asr->conv_x_t_pad->w + j + asr->lorder] = conv_x_t_data[i * asr->conv_x_t->w + j];
                    }
                }
                // x = nn.functional.pad(x, (self.lorder, 0), 'constant', 0.0)
            } else {
                for (int i = 0; i < asr->conv_x_t_pad->h; i ++) {
                    for (int j = 0; j < asr->lorder; j ++) {
                        conv_x_t_pad_data[i * asr->conv_x_t_pad->w + j] = conv_cache_data[i * asr->conv_cache[layer]->w + j];
                    }
                    for (int j = 0; j < asr->sub4_conv_conv2d_2_h_out; j ++) {
                        conv_x_t_pad_data[i * asr->conv_x_t_pad->w + j + asr->lorder] = conv_x_t_data[i * asr->conv_x_t->w + j];
                    }
                }
                // x = torch.cat((cache, x), dim=2)
            }
            // Mat_2D_float_printf(asr->conv_x_t_pad);
            
            for (int i = 0; i < asr->conv_cache[layer]->h; i ++) {
                for (int j = 0; j < asr->lorder; j ++) {
                    conv_cache_data[i * asr->conv_cache[layer]->w + j] = conv_x_t_pad_data[i * asr->conv_x_t_pad->w + j + asr->sub4_conv_conv2d_2_h_out];
                }
            }
            // new_cache = x[:, :, -self.lorder:]
            // Mat_2D_float_printf(asr->conv_cache[layer]);

            Conv1D_process(asr->conformer_conv_module_pointwise_conv1[layer], asr->conv_x_t_pad);
            Mat *conformer_conv_module_pointwise_conv1_out = Conv1D_get_output(asr->conformer_conv_module_pointwise_conv1[layer]);
            // x = self.pointwise_conv1(x)  # (batch, 2*channel, dim)
            // Mat_2D_float_printf(conformer_conv_module_pointwise_conv1_out);

            GLU_process(asr->conformer_conv_module_glu, conformer_conv_module_pointwise_conv1_out);
            Mat *conformer_conv_module_glu_out = GLU_get_output(asr->conformer_conv_module_glu);
            // x = nn.functional.glu(x, dim=1)  # (batch, channel, dim)
            // Mat_2D_float_printf(conformer_conv_module_glu_out);

            Depthwise_Conv1D_process(asr->conformer_conv_module_depthwise_conv[layer], conformer_conv_module_glu_out);
            Mat *conformer_conv_module_depthwise_conv_out = Depthwise_Conv1D_get_output(asr->conformer_conv_module_depthwise_conv[layer]);
            // x = self.depthwise_conv(x)
            // Mat_2D_float_printf(conformer_conv_module_depthwise_conv_out);

            Permute_process(conformer_conv_module_depthwise_conv_out, asr->depthwise_conv_out_t, 1);
            // printf("%d, %d, %d\n", asr->depthwise_conv_out_t->w, asr->depthwise_conv_out_t->h, asr->depthwise_conv_out_t->c);
            LayerNorm_process_inplace(asr->conformer_conv_module_norm[layer], asr->depthwise_conv_out_t);
            SiLU_process_inplace(asr->conformer_conv_module_act, asr->depthwise_conv_out_t);
            // x = self.activation(self.norm(x))
            // Mat_2D_float_printf(asr->depthwise_conv_out_t);
            Permute_process(asr->depthwise_conv_out_t, asr->conformer_conv_module_act_out_t, 1);
            // printf("%d, %d, %d\n", conformer_conv_module_act_out_t->w, conformer_conv_module_act_out_t->h, conformer_conv_module_act_out_t->c);

            Conv1D_process(asr->conformer_conv_module_pointwise_conv2[layer], asr->conformer_conv_module_act_out_t);
            Mat *conformer_conv_module_pointwise_conv2_out = Conv1D_get_output(asr->conformer_conv_module_pointwise_conv2[layer]);  
            // x = self.pointwise_conv2(x)      
            // Mat_2D_float_printf(conformer_conv_module_pointwise_conv2_out);
            Permute_process(conformer_conv_module_pointwise_conv2_out, asr->conv_out, 1);

            for (int i = 0; i < asr->conv_out->h; i ++) {
                for (int j = 0; j < asr->conv_out->w; j ++) {
                    ((float *)asr->conv_out->data)[i * asr->conv_out->w + j] = \
                    ((float *)asr->conformer_residual->data)[i * asr->conv_out->w + j] + \
                    ((float *)asr->conv_out->data)[i * asr->conv_out->w + j];
                }
            }
            // x = residual + self.dropout(x)
            // Mat_2D_float_printf(asr->conv_out);

            // 4.feed forward module
            memcpy(asr->conformer_residual->data, asr->conv_out->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            LayerNorm_process_inplace(asr->conformer_norm_ff[layer], asr->conv_out);
            // x = self.norm_ff(x)x = self.norm_ff(x)
            // Mat_2D_float_printf(asr->conv_out);
            
            Linear_process(asr->conformer_feed_forward_w_1[layer], asr->conv_out);
            Mat *conformer_feed_forward_w_1_out = Linear_get_output(asr->conformer_feed_forward_w_1[layer]);
            // Mat_2D_float_printf(conformer_feed_forward_w_1_out);

            SiLU_process_inplace(asr->conformer_feed_forward_act, conformer_feed_forward_w_1_out);
            // Mat_2D_float_printf(conformer_feed_forward_w_1_out);

            Linear_process(asr->conformer_feed_forward_w_2[layer], conformer_feed_forward_w_1_out);
            Mat *conformer_feed_forward_w_2_out = Linear_get_output(asr->conformer_feed_forward_w_2[layer]);
            // Mat_2D_float_printf(conformer_feed_forward_w_2_out);

            for (int i = 0; i < conformer_feed_forward_w_2_out->h; i ++) {
                for (int j = 0; j < conformer_feed_forward_w_2_out->w; j ++) {
                    ((float *)conformer_feed_forward_w_2_out->data)[i * conformer_feed_forward_w_2_out->w + j] = \
                    ((float *)asr->conformer_residual->data)[i * conformer_feed_forward_w_2_out->w + j] + \
                    ((float *)conformer_feed_forward_w_2_out->data)[i * conformer_feed_forward_w_2_out->w + j] * asr->ff_scale;
                }
            }
            // x = residual + self.ff_scale * self.dropout(self.feed_forward(x))
            // Mat_2D_float_printf(conformer_feed_forward_w_2_out);

            LayerNorm_process_inplace(asr->conformer_norm_final[layer], conformer_feed_forward_w_2_out);
            // x = self.norm_final(x)
            // Mat_2D_float_printf(conformer_feed_forward_w_2_out);

            memcpy(sub4_out_linear_out->data, conformer_feed_forward_w_2_out->data, sizeof(float) * asr->odim * asr->sub4_conv_conv2d_2_h_out);
            // printf("%d, %d, %d\n", sub4_out_linear_out->w, sub4_out_linear_out->h, sub4_out_linear_out->c);

        } // end conformer encoder
        
        // printf("%d, %d, %d\n", sub4_out_linear_out->w, sub4_out_linear_out->h, sub4_out_linear_out->c);
        LayerNorm_process_inplace(asr->conformer_after_norm, sub4_out_linear_out);
        // Mat_2D_float_printf(sub4_out_linear_out);
        // xs = self.after_norm(xs)

        Linear_process(asr->ctc_lo, sub4_out_linear_out);
        Mat *ctc_lo_out = Linear_get_output(asr->ctc_lo);
        // Mat_2D_float_printf(ctc_lo_out);
        // if (asr->chunk_xs_idx == 17) {
        //     Mat_2D_float_printf(ctc_lo_out);
        // }
        // self.ctc_lo = torch.nn.Linear(eprojs, odim)

        Softmax_process_inplace(asr->ctc_lo_softmax, ctc_lo_out);
        // Mat_2D_float_printf(ctc_lo_out);
        
        for (int i = 0; i < ctc_lo_out->h; i ++) {
            // float sum = 0;
            for (int j = 0; j < ctc_lo_out->w; j ++) {
                // sum += ((float *)ctc_lo_out->data)[i * ctc_lo_out->w + j];
                ((float *)ctc_lo_out->data)[i * ctc_lo_out->w + j] = log(((float *)ctc_lo_out->data)[i * ctc_lo_out->w + j]);
            }
            // printf("%f\n", sum);
        }
        // printf("%d, %d, %d\n", ctc_lo_out->w, ctc_lo_out->h, ctc_lo_out->c);
        // Mat_2D_float_printf(ctc_lo_out);


        // ****** ctc_prefix_beam_search ******
        int blank = 0;
        int first_beam_size = 10;
        int second_beam_size = 10;

        // asr->ctc_decoder = ctc_prefix_beam_search_create(ctc_lo_out->h, asr->vocab_size, blank, first_beam_size, second_beam_size);
        asr->ctc_decoder_wrt_timestamp = ctc_prefix_beam_search_wrt_timestamp_create(ctc_lo_out->h, asr->vocab_size, blank, first_beam_size, second_beam_size);

        float **ctc_probs = (float **)malloc(sizeof(float *) * ctc_lo_out->h);
    	for (int i = 0; i < ctc_lo_out->h; i ++) {
            ctc_probs[i] = (float *)malloc(sizeof(float) * asr->vocab_size);
        }

        for (int i = 0; i < ctc_lo_out->h; i ++) {
            for (int j = 0; j < ctc_lo_out->w; j ++) {
                ctc_probs[i][j] = ((float *)ctc_lo_out->data)[i * ctc_lo_out->w + j];
            }
        }

        int *hyps = (int *)malloc(sizeof(int) * ctc_lo_out->h);
        // int best_prefix_dynamic_size = ctc_prefix_beam_search_process(asr->ctc_decoder, ctc_probs, hyps);
        int *hyps_times = (int *)malloc(sizeof(int) * ctc_lo_out->h);
        int best_prefix_dynamic_size = ctc_prefix_beam_search_wrt_timestamp_process(asr->ctc_decoder_wrt_timestamp, ctc_probs, hyps, hyps_times, cont_time_step);

        int ts_res_msg_char_num = 1;
        char *ts_res_msg = (char *)malloc(sizeof(char) * ts_res_msg_char_num);
        ts_res_msg[0] = '\0';

        if (best_prefix_dynamic_size > 0) {
            for (int i = 0; i < best_prefix_dynamic_size; i ++) {
                int start = (hyps_times[i] - cont_time_step) * frame_shift_in_ms > time_stamp_gap ? hyps_times[i] * frame_shift_in_ms - time_stamp_gap : cont_time_step * frame_shift_in_ms;

                if (i > 0) {
                    start = (hyps_times[i] - hyps_times[i - 1]) * frame_shift_in_ms < time_stamp_gap ? (hyps_times[i - 1] + hyps_times[i]) / 2 * frame_shift_in_ms : start;
                }

                int end = hyps_times[i] * frame_shift_in_ms;

                if (i < best_prefix_dynamic_size - 1) {
                    end = (hyps_times[i + 1] - hyps_times[i]) * frame_shift_in_ms < time_stamp_gap ? (hyps_times[i + 1] + hyps_times[i]) / 2 * frame_shift_in_ms : end;
                }
                
                // word_level_post_process(char_dict[hyps[i]].value);
                printf("timestamp: %d\tword: \"%s\"\tstart: %d\tend: %d\n", hyps_times[i], char_dict[hyps[i]].value, start, end);

                char *start_str = (char *)malloc(sizeof(char) * 256);
                char *end_str = (char *)malloc(sizeof(char) * 256);
                sprintf(start_str, "%d", start);
                sprintf(end_str, "%d", end);
                // printf("%ld\t%ld\n", strlen(start_str), strlen(end_str));

                ts_res_msg_char_num += 1;
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, "{");
                ts_res_msg_char_num += 1;
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, "<");
                ts_res_msg_char_num += strlen(char_dict[hyps[i]].value);
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, char_dict[hyps[i]].value);
                ts_res_msg_char_num += 1;
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, ">");
                ts_res_msg_char_num += 1;
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, "<");
                ts_res_msg_char_num += strlen(start_str);
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, start_str);
                ts_res_msg_char_num += 1;
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, ">");
                ts_res_msg_char_num += 1;
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, "<");
                ts_res_msg_char_num += strlen(end_str);
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, end_str);
                ts_res_msg_char_num += 1;
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, ">");
                ts_res_msg_char_num += 1;
                ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
                strcat(ts_res_msg, "}");

                free(start_str);
                start_str = NULL;
                free(end_str);
                end_str = NULL;
            }
        } else {
            int start = cont_time_step * frame_shift_in_ms;
            int end = (cont_time_step + ctc_lo_out->h) * frame_shift_in_ms;

            printf("timestamp: %s\tword: \"%s\"\tstart: %d\tend: %d\n", "", "", start, end);

            char *start_str = (char *)malloc(sizeof(char) * 256);
            char *end_str = (char *)malloc(sizeof(char) * 256);
            sprintf(start_str, "%d", start);
            sprintf(end_str, "%d", end);
            // printf("%ld\t%ld\n", strlen(start_str), strlen(end_str));

            ts_res_msg_char_num += 1;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, "{");
            ts_res_msg_char_num += 1;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, "<");
            ts_res_msg_char_num += 0;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, "");
            ts_res_msg_char_num += 1;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, ">");
            ts_res_msg_char_num += 1;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, "<");
            ts_res_msg_char_num += strlen(start_str);
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, start_str);
            ts_res_msg_char_num += 1;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, ">");
            ts_res_msg_char_num += 1;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, "<");
            ts_res_msg_char_num += strlen(end_str);
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, end_str);
            ts_res_msg_char_num += 1;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, ">");
            ts_res_msg_char_num += 1;
            ts_res_msg = (char *)realloc(ts_res_msg, sizeof(char) * ts_res_msg_char_num);
            strcat(ts_res_msg, "}");

            free(start_str);
            start_str = NULL;
            free(end_str);
            end_str = NULL;
        }
        printf("%s\n", ts_res_msg);
        // printf("%ld\n", strlen(ts_res_msg));

        // load dict
        int content_char_num = 1;
        char *content = (char *)malloc(sizeof(char) * content_char_num);
        content[0] = '\0';
        for (int i = 0; i < best_prefix_dynamic_size; i ++) {
            content_char_num += strlen(char_dict[hyps[i]].value);
            content = (char *)realloc(content, sizeof(char) * content_char_num);
            strcat(content, char_dict[hyps[i]].value);
        }
        // printf("%s\n", content);
        // printf("%ld\n", strlen(content));
        
        res_msg_char_num += strlen(content);
        res_msg = (char *)realloc(res_msg, sizeof(char) * res_msg_char_num);
        strcat(res_msg, content);
        sentence_level_post_process(res_msg);
        printf("%s\n", res_msg);

        int res_per_space_msg_count = 0;
        char **res_per_space_msg = NULL;
        for (int i = 0; i < strlen(res_msg); i ++) {
            if (*(res_msg + i) != ' ') {
                res_per_space_msg_count ++;
                res_per_space_msg = (char **)realloc(res_per_space_msg, sizeof(char *) * res_per_space_msg_count);

                int res_per_space_msg_index_size = 0;
                res_per_space_msg[res_per_space_msg_count - 1] = NULL;
                while (*(res_msg + i) != ' '&& i < strlen(res_msg)) {
                    res_per_space_msg_index_size ++;
                    res_per_space_msg[res_per_space_msg_count - 1] = (char *)realloc(res_per_space_msg[res_per_space_msg_count - 1], sizeof(char) * res_per_space_msg_index_size);
                    res_per_space_msg[res_per_space_msg_count - 1][res_per_space_msg_index_size - 1] = *(res_msg + i);
                    
                    i ++;
                }
                res_per_space_msg[res_per_space_msg_count - 1] = (char *)realloc(res_per_space_msg[res_per_space_msg_count - 1], sizeof(char) * (res_per_space_msg_index_size + 1));
                res_per_space_msg[res_per_space_msg_count - 1][res_per_space_msg_index_size] = '\0';
            }
        }

        int *res_size = (int *)malloc(sizeof(int));
        char **res_msg_out = InverseTextNormalization(res_per_space_msg, res_per_space_msg_count, res_size);

        printf("Inverse Text Normalization Result:\n");
        for (int i = 0; i < res_size[0]; i ++) {
            printf("%s", res_msg_out[i]);
            if (i < res_size[0] - 1) { printf(" "); } else { printf("\n"); }
        }
        
        for (int i = 0; i < res_per_space_msg_count; i ++) {
            free(res_per_space_msg[i]);
            res_per_space_msg[i] = NULL;
        }
        free(res_per_space_msg);
        res_per_space_msg = NULL;

        for (int i = 0; i < res_size[0]; i ++) {
            free(res_msg_out[i]);
            res_msg_out[i] = NULL;
        }
        free(res_msg_out);
        res_msg_out = NULL;
        free(res_size);
        res_size = NULL;

        // ctc_prefix_beam_search_destroy(asr->ctc_decoder);
        ctc_prefix_beam_search_wrt_timestamp_destroy(asr->ctc_decoder_wrt_timestamp);

        cont_time_step += ctc_lo_out->h;
        // ****** ctc_prefix_beam_search ******

        asr->chunk_xs_idx ++;

        // ****** ctc_endpoint ******
        ctc_endpoint_probs_size += ctc_lo_out->h;
        ctc_endpoint_probs = (float **)realloc(ctc_endpoint_probs, sizeof(float *) * ctc_endpoint_probs_size);
    	for (int i = ctc_endpoint_probs_size - ctc_lo_out->h; i < ctc_endpoint_probs_size; i ++) {
            ctc_endpoint_probs[i] = (float *)malloc(sizeof(float) * asr->vocab_size);
        }

    	for (int i = ctc_endpoint_probs_size - ctc_lo_out->h; i < ctc_endpoint_probs_size; i ++) {
            for (int j = 0; j < ctc_lo_out->w; j ++) {
                ctc_endpoint_probs[i][j] = ctc_probs[i - ctc_endpoint_probs_size + ctc_lo_out->h][j];
            }
        }

        float blank_threshold = 0.999995;

        asr->ctc_endpoint_detector = ctc_endpoint_create(ctc_endpoint_probs_size, asr->vocab_size, blank, blank_threshold, frame_shift_in_ms);
        // printf("%d\n", asr->ctc_endpoint_detector->num_frames_decoded);
        // printf("%d\n", asr->ctc_endpoint_detector->num_frames_trailing_blank);

        if (content != NULL && strlen(content) != 0) { asr->decoded_something = 1; }
        int is_endpoint = ctc_endpoint_process(asr->ctc_endpoint_detector, ctc_endpoint_probs, asr->decoded_something);
        // printf("%d\n", is_endpoint);
        if (is_endpoint) {
            if (is_endpoint == 2 || is_endpoint == 3) { // rule2 or rule3
                res_msg_char_num += 1;
                res_msg = (char *)realloc(res_msg, sizeof(char) * res_msg_char_num);
                strcat(res_msg, " ,");
                printf("%s\n", res_msg);

                int res_per_space_msg_count_endpoint = 0;
                char **res_per_space_msg_endpoint = NULL;
                for (int i = 0; i < strlen(res_msg); i ++) {
                    if (*(res_msg + i) != ' ') {
                        res_per_space_msg_count_endpoint ++;
                        res_per_space_msg_endpoint = (char **)realloc(res_per_space_msg_endpoint, sizeof(char *) * res_per_space_msg_count_endpoint);

                        int res_per_space_msg_index_size_endpoint = 0;
                        res_per_space_msg_endpoint[res_per_space_msg_count_endpoint - 1] = NULL;
                        while (*(res_msg + i) != ' '&& i < strlen(res_msg)) {
                            res_per_space_msg_index_size_endpoint ++;
                            res_per_space_msg_endpoint[res_per_space_msg_count_endpoint - 1] = (char *)realloc(res_per_space_msg_endpoint[res_per_space_msg_count_endpoint - 1], sizeof(char) * res_per_space_msg_index_size_endpoint);
                            res_per_space_msg_endpoint[res_per_space_msg_count_endpoint - 1][res_per_space_msg_index_size_endpoint - 1] = *(res_msg + i);
                            
                            i ++;
                        }
                        res_per_space_msg_endpoint[res_per_space_msg_count_endpoint - 1] = (char *)realloc(res_per_space_msg_endpoint[res_per_space_msg_count_endpoint - 1], sizeof(char) * (res_per_space_msg_index_size_endpoint + 1));
                        res_per_space_msg_endpoint[res_per_space_msg_count_endpoint - 1][res_per_space_msg_index_size_endpoint] = '\0';
                    }
                }

                int *res_size_endpoint = (int *)malloc(sizeof(int));
                char **res_msg_out_endpoint = InverseTextNormalization(res_per_space_msg_endpoint, res_per_space_msg_count_endpoint, res_size_endpoint);

                printf("Endpoint Inverse Text Normalization Result:\n");
                for (int i = 0; i < res_size_endpoint[0]; i ++) {
                    printf("%s", res_msg_out_endpoint[i]);
                    if (i < res_size_endpoint[0] - 1) { printf(" "); } else { printf("\n"); }
                }
                
                for (int i = 0; i < res_per_space_msg_count_endpoint; i ++) {
                    free(res_per_space_msg_endpoint[i]);
                    res_per_space_msg_endpoint[i] = NULL;
                }
                free(res_per_space_msg_endpoint);
                res_per_space_msg_endpoint = NULL;

                for (int i = 0; i < res_size_endpoint[0]; i ++) {
                    free(res_msg_out_endpoint[i]);
                    res_msg_out_endpoint[i] = NULL;
                }
                free(res_msg_out_endpoint);
                res_msg_out_endpoint = NULL;
                free(res_size_endpoint);
                res_size_endpoint = NULL;
            }

            free(res_msg);
            res_msg = NULL;

            res_msg_char_num = 1;
            res_msg = (char *)malloc(sizeof(char) * res_msg_char_num);
            res_msg[0] = '\0';

            for (int i = 0; i < ctc_endpoint_probs_size; i ++) {
                free(ctc_endpoint_probs[i]);
                ctc_endpoint_probs[i] = NULL;
            }
            free(ctc_endpoint_probs);
            ctc_endpoint_probs = NULL;

            ctc_endpoint_probs_size = 0;

            asr->decoded_something = 0;

            for (int i = 0; i < num_encoder_layer; i ++) {
                for (int j = 0; j < k_v_cache_max; j ++) {
                    Mat_destroy(asr->k_cache[i][j]);
                    Mat_destroy(asr->v_cache[i][j]);
                }
            }

            for (int i = 0; i < num_encoder_layer; i ++) {
                for (int j = 0; j < k_v_cache_max; j ++) {
                    asr->k_cache[i][j] = Mat_3D_create(asr->d_k, (j + 1) * asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
                    asr->v_cache[i][j] = Mat_3D_create(asr->d_k, (j + 1) * asr->sub4_conv_conv2d_2_h_out, asr->head, 4, 1);
                }
            }

            for (int i = 0; i < num_encoder_layer; i ++) {
                Mat_destroy(asr->conv_cache[i]);
            }

            for (int i = 0; i < num_encoder_layer; i ++) {
                asr->conv_cache[i] = Mat_2D_create(asr->lorder, asr->odim, 4, 1);
            }

            asr->chunk_xs_idx = 0;

            asr->offset = 0;
        }

        ctc_endpoint_destroy(asr->ctc_endpoint_detector);
        // ****** ctc_endpoint ******

        Mat_destroy(pos_emb);

        Mat_destroy(conformer_self_attn_linear_pos_out);
        Mat_destroy(conformer_self_attn_linear_pos_out_view);
        Mat_destroy(conformer_self_attn_linear_pos_out_view_t);

        Mat_destroy(matrix_ac);
        Mat_destroy(matrix_bd);

        for (int i = 0; i < ctc_lo_out->h; i ++) {
            free(ctc_probs[i]);
            ctc_probs[i] = NULL;
        }
        free(ctc_probs);
        ctc_probs = NULL;

        free(hyps);
        hyps = NULL;
        free(content);
        content = NULL;
        free(ts_res_msg);
        ts_res_msg = NULL;
        
        #if true
        gettimeofday(&time_end, NULL);
        long long total_time = (time_end.tv_sec - time_start.tv_sec) * 1000000 + (time_end.tv_usec - time_start.tv_usec);
        printf("total time is %lld ms\n\n", total_time / 1000);
        #endif
    } // cur end

    for (int i = 0; i < num_frames; i ++) {
        free(feat[i]);
        feat[i] = NULL;
    }
    free(feat);
    feat = NULL;

    free(res_msg);
    res_msg = NULL;

    return 0;
}

int wohospeech_asr_destroy(WoHoSpeech_ASR *asr) {
    if (asr != NULL) {
        fbank_destroy(asr->fb);
        Mat_destroy(asr->chunk_xs);
        
        free(asr);
        asr = NULL;
    }

    return 0;
}
