#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "gru.h"

GRU *GRU_create(int input_size, int hidden_size, bool bias_used) {
    GRU *gru = (GRU *)malloc(sizeof(GRU));
    if (gru == NULL) {
        return NULL;
    }

    gru->input_size = input_size;
    gru->hidden_size = hidden_size;
    gru->bias_used = bias_used;

    gru->weight_xc_mat = Mat_2D_create(input_size, 3 * hidden_size, 4, 1);
    gru->weight_hc_mat = Mat_2D_create(hidden_size, 3 * hidden_size, 4, 1);
    gru->bias_c_mat = Mat_2D_create(hidden_size, 4, 4, 1);

    gru->hidden_mat = Mat_1D_create(hidden_size, 4, 1);

    float *hidden_ptr = (float *)gru->hidden_mat->data;
    for (int i = 0; i < hidden_size; i ++) {
        hidden_ptr[i] = 0.0f;
    }

    gru->gates_mat = Mat_2D_create(2, hidden_size, 4, 1);

    gru->out_mat = NULL;

    return gru;
}

int GRU_load_variables(GRU *gru, char *file) {
    if (gru == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(gru->weight_xc_mat->data, sizeof(float), gru->input_size * gru->hidden_size * 3, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    
    // Mat_2D_float_printf(gru->weight_xc_mat);
    
    ret = fread(gru->weight_hc_mat->data, sizeof(float), gru->hidden_size * gru->hidden_size * 3, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    
    // Mat_2D_float_printf(gru->weight_hc_mat);

    if (gru->bias_used == true) {
        ret = fread(gru->bias_c_mat->data, sizeof(float), gru->hidden_size * 4, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }
        // Mat_2D_float_printf(gru->bias_c_mat);
    }

    fclose(weight_bin_file);

    return 0;
}

int GRU_dump_header(GRU *gru, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name) {
    if (gru == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(param_file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(gru->weight_xc_mat->data, sizeof(float), gru->input_size * gru->hidden_size * 3, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    
    
    ret = fread(gru->weight_hc_mat->data, sizeof(float), gru->hidden_size * gru->hidden_size * 3, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    

    if (gru->bias_used == true) {
        ret = fread(gru->bias_c_mat->data, sizeof(float), gru->hidden_size * 4, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }
    }

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "weight_xc");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "weight_xc");
    float *data_ptr = gru->weight_xc_mat->data;
    for (int i = 0; i < gru->input_size * gru->hidden_size * 3; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");
    
    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "weight_hc");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "weight_hc");
    data_ptr = gru->weight_hc_mat->data;
    for (int i = 0; i < gru->hidden_size * gru->hidden_size * 3; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "bias");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "bias");
    data_ptr = gru->bias_c_mat->data;
    for (int i = 0; i < gru->hidden_size * 4; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fclose(weight_bin_file);

    return 0;
}

int GRU_init_variables(GRU *gru, float *weight_xc, float *weight_hc, float *bias) {
    if (gru == NULL) {
        return -1;
    }

    memcpy(gru->weight_xc_mat->data, weight_xc, sizeof(float) * gru->input_size * gru->hidden_size * 3);
    // Mat_2D_float_printf(lstm->weight_xc_mat);

    memcpy(gru->weight_hc_mat->data, weight_hc, sizeof(float) * gru->hidden_size * gru->hidden_size * 3);
    // Mat_2D_float_printf(lstm->weight_hc_mat);

    if (gru->bias_used == true) {
        memcpy(gru->bias_c_mat->data, bias, sizeof(float) * gru->hidden_size * 4);
    }

    return 0;
}

int GRU_process(GRU *gru, Mat *input) {
    if (gru == NULL) {
        return -1;
    }

    if (gru->out_mat == NULL) {
        gru->out_mat = Mat_2D_create(gru->hidden_size, input->h, sizeof(float), 1);
    }

    int T = input->h;
    int num_output = gru->hidden_size;
    float *hidden_state = (float *)gru->hidden_mat->data;

    for (int t = 0; t < T; t ++) {
        const float *x = (float *)Mat_row(input, t);

        for (int q = 0; q < num_output; q ++) {
            float *gates_data = (float *)Mat_row(gru->gates_mat, q);

            // gate reset update
            const float *bias_c_R = (float *)Mat_row(gru->bias_c_mat, 0);
            const float *bias_c_U = (float *)Mat_row(gru->bias_c_mat, 1);

            const float *weight_xc_R = (float *)Mat_row(gru->weight_xc_mat, num_output * 0 + q);
            const float *weight_xc_U = (float *)Mat_row(gru->weight_xc_mat, num_output * 1 + q);
            const float *weight_hc_R = (float *)Mat_row(gru->weight_hc_mat, num_output * 0 + q);
            const float *weight_hc_U = (float *)Mat_row(gru->weight_hc_mat, num_output * 1 + q);

            float R = bias_c_R[q];
            float U = bias_c_U[q];

            for (int i = 0; i < gru->input_size; i ++) {
                float xi = x[i];

                R += weight_xc_R[i] * xi;
                U += weight_xc_U[i] * xi;
            }

            for (int i = 0; i < num_output; i ++) {
                float h_cont = hidden_state[i];

                R += weight_hc_R[i] * h_cont;
                U += weight_hc_U[i] * h_cont;
            }

            // sigmoid(R)
            // sigmoid(U)
            R = 1.f / (1.f + exp(-R));
            U = 1.f / (1.f + exp(-U));

            // gate new
            const float *bias_c_WN = (float *)Mat_row(gru->bias_c_mat, 2);
            const float *bias_c_BN = (float *)Mat_row(gru->bias_c_mat, 3);

            const float *weight_xc_N = (float *)Mat_row(gru->weight_xc_mat, num_output * 2 + q);
            const float *weight_hc_N = (float *)Mat_row(gru->weight_hc_mat, num_output * 2 + q);

            float N = bias_c_BN[q];

            for (int i = 0; i < num_output; i ++) {
                float h_cont = hidden_state[i];

                N += weight_hc_N[i] * h_cont;
            }

            N = bias_c_WN[q] + R * N;

            for (int i = 0; i < gru->input_size; i ++) {
                float xi = x[i];

                N += weight_xc_N[i] * xi;
            }

            // tanh(N)
            N = tanh(N);

            gates_data[0] = U;
            gates_data[1] = N;
        }

        // h_t := (1 - update) .* new + update .* h_{t-1}
        float *output_data = (float *)Mat_row(gru->out_mat, t);
        for (int q = 0; q < num_output; q ++) {
            const float *gates_data = (float *)Mat_row(gru->gates_mat, q);

            float U = gates_data[0];
            float N = gates_data[1];

            float H = (1 - U) * N + U * hidden_state[q];

            hidden_state[q] = H;
            output_data[q] = H;
            // printf("%f ", H);
        }
        // printf("\n");
        // getchar();
    }

    return 0;
}

Mat *GRU_get_output(GRU *gru) {
    return gru->out_mat;
}

int GRU_destroy(GRU *gru) {
    if (gru != NULL) {
        Mat_destroy(gru->weight_xc_mat);
        Mat_destroy(gru->weight_hc_mat);
        Mat_destroy(gru->bias_c_mat);
        Mat_destroy(gru->hidden_mat);
        Mat_destroy(gru->gates_mat);
        if (gru->out_mat != NULL) {
            Mat_destroy(gru->out_mat);
        }

        free(gru);
        gru = NULL;
    }
    
    return 0;
}