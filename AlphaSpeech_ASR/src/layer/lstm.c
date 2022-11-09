#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "lstm.h"

LSTM *LSTM_create(int input_size, int hidden_size, bool bias_used) {
    LSTM *lstm = (LSTM *)malloc(sizeof(LSTM));
    if (lstm == NULL) {
        return NULL;
    }

    lstm->input_size = input_size;
    lstm->hidden_size = hidden_size;
    lstm->bias_used = bias_used;

    lstm->weight_xc_mat = Mat_2D_create(input_size, 4 * hidden_size, 4, 1);
    lstm->weight_hc_mat = Mat_2D_create(hidden_size, 4 * hidden_size, 4, 1);
    lstm->bias_c_mat = Mat_2D_create(hidden_size, 4, 4, 1);

    lstm->hidden_mat = Mat_1D_create(hidden_size, 4, 1);
    lstm->cell_mat = Mat_1D_create(hidden_size, 4, 1);

    float *hidden_ptr = (float *)lstm->hidden_mat->data;
    float *cell_ptr = (float *)lstm->cell_mat->data;
    for (int i = 0; i < hidden_size; i ++) {
        hidden_ptr[i] = 0.0f;
        cell_ptr[i] = 0.0f;
    }

    lstm->gates_mat = Mat_2D_create(4, hidden_size, 4, 1);

    lstm->out_mat = NULL;

    return lstm;
}

int LSTM_load_variables(LSTM *lstm, char *file) {
    if (lstm == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(lstm->weight_xc_mat->data, sizeof(float), lstm->input_size * lstm->hidden_size * 4, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    
    // Mat_2D_float_printf(lstm->weight_xc_mat);
    
    ret = fread(lstm->weight_hc_mat->data, sizeof(float), lstm->hidden_size * lstm->hidden_size * 4, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    
    // Mat_2D_float_printf(lstm->weight_hc_mat);

    if (lstm->bias_used == true) {
        ret = fread(lstm->bias_c_mat->data, sizeof(float), lstm->hidden_size * 4, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }
        // Mat_2D_float_printf(lstm->bias_c_mat);
    }

    fclose(weight_bin_file);

    return 0;
}

int LSTM_init_variables(LSTM *lstm, float *weight_xc, float *weight_hc, float *bias) {
    if (lstm == NULL) {
        return -1;
    }

    memcpy(lstm->weight_xc_mat->data, weight_xc, sizeof(float) * lstm->input_size * lstm->hidden_size * 4);
    // Mat_2D_float_printf(lstm->weight_xc_mat);

    memcpy(lstm->weight_hc_mat->data, weight_hc, sizeof(float) * lstm->hidden_size * lstm->hidden_size * 4);
    // Mat_2D_float_printf(lstm->weight_hc_mat);

    if (lstm->bias_used == true) {
        memcpy(lstm->bias_c_mat->data, bias, sizeof(float) * lstm->hidden_size * 4);
    }

    return 0;
}

int LSTM_process(LSTM *lstm, Mat *input) {
    if (lstm == NULL) {
        return -1;
    }

    if (lstm->out_mat == NULL) {
        lstm->out_mat = Mat_2D_create(lstm->hidden_size, input->h, sizeof(float), 1);
    }

    int T = input->h;
    int num_output = lstm->hidden_size;
    float *hidden_state = (float *)lstm->hidden_mat->data;
    float *cell_state = (float *)lstm->cell_mat->data;

    for (int t = 0; t < T; t ++) {
        // clip hidden by continuation indicator
        // h_cont_{t-1} = cont_t * h_{t-1}
        // h_cont_{t-1} = h_{t-1} if cont_t == 1
        //                0       otherwise
        // calculate hidden
        // gate_input_t := W_hc * h_conted_{t-1} + W_xc * x_t + b_c

        const float *x = (float *)Mat_row(input, t);
        for (int q = 0; q < num_output; q ++) {
            const float *bias_c_I = (float *)Mat_row(lstm->bias_c_mat, 0);
            const float *bias_c_F = (float *)Mat_row(lstm->bias_c_mat, 1);
            const float *bias_c_O = (float *)Mat_row(lstm->bias_c_mat, 3);
            const float *bias_c_G = (float *)Mat_row(lstm->bias_c_mat, 2);

            float *gates_data = (float *)Mat_row(lstm->gates_mat, q);

            // gate I F O G
            const float *weight_xc_I = (float *)Mat_row(lstm->weight_xc_mat, num_output * 0 + q);
            const float *weight_xc_F = (float *)Mat_row(lstm->weight_xc_mat, num_output * 1 + q);
            const float *weight_xc_O = (float *)Mat_row(lstm->weight_xc_mat, num_output * 3 + q);
            const float *weight_xc_G = (float *)Mat_row(lstm->weight_xc_mat, num_output * 2 + q);

            const float *weight_hc_I = (float *)Mat_row(lstm->weight_hc_mat, num_output * 0 + q);
            const float *weight_hc_F = (float *)Mat_row(lstm->weight_hc_mat, num_output * 1 + q);
            const float *weight_hc_O = (float *)Mat_row(lstm->weight_hc_mat, num_output * 3 + q);
            const float *weight_hc_G = (float *)Mat_row(lstm->weight_hc_mat, num_output * 2 + q);

            float I = bias_c_I[q];
            float F = bias_c_F[q];
            float O = bias_c_O[q];
            float G = bias_c_G[q];

            for (int i = 0; i < lstm->input_size; i ++) {
                float xi = x[i];

                I += weight_xc_I[i] * xi;
                F += weight_xc_F[i] * xi;
                O += weight_xc_O[i] * xi;
                G += weight_xc_G[i] * xi;
            }

            for (int i = 0; i < num_output; i ++) {
                float h_cont = hidden_state[i];

                I += weight_hc_I[i] * h_cont;
                F += weight_hc_F[i] * h_cont;
                O += weight_hc_O[i] * h_cont;
                G += weight_hc_G[i] * h_cont;
            }

            gates_data[0] = I;
            gates_data[1] = F;
            gates_data[2] = O;
            gates_data[3] = G;
        }

        // lstm unit
        // sigmoid(I)
        // sigmoid(F)
        // sigmoid(O)
        // tanh(G)
        // c_t := f_t .* c_{t-1} + i_t .* g_t
        // h_t := o_t .* tanh[c_t]
        float *output_data = (float *)Mat_row(lstm->out_mat, t);
        for (int q = 0; q < num_output; q ++) {
            const float *gates_data = (float *)Mat_row(lstm->gates_mat, q);

            float I = gates_data[0];
            float F = gates_data[1];
            float O = gates_data[2];
            float G = gates_data[3];

            I = 1.f / (1.f + exp(-I));
            F = 1.f / (1.f + exp(-F));
            O = 1.f / (1.f + exp(-O));
            G = tanh(G);

            float cell2 = F * cell_state[q] + I * G;
            float H = O * tanh(cell2);
            cell_state[q] = cell2;
            hidden_state[q] = H;
            output_data[q] = H;
        }
    }

    return 0;
}

Mat *LSTM_get_output(LSTM *lstm) {
    return lstm->out_mat;
}

int LSTM_destroy(LSTM *lstm) {
    if (lstm != NULL) {
        Mat_destroy(lstm->weight_xc_mat);
        Mat_destroy(lstm->weight_hc_mat);
        Mat_destroy(lstm->bias_c_mat);
        Mat_destroy(lstm->hidden_mat);
        Mat_destroy(lstm->cell_mat);
        Mat_destroy(lstm->gates_mat);
        if (lstm->out_mat != NULL) {
            Mat_destroy(lstm->out_mat);
        }

        free(lstm);
        lstm = NULL;
    }
    
    return 0;
}