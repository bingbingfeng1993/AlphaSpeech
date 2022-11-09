import imp
from tokenize import String
import os
import torch
import struct
import numpy as np

def param_dump(param_dump,save_file:String):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    param = np.float32(param_dump.detach().cpu().numpy())
    param = list(param.reshape(-1))
    print("param len:" + str(len(param)))
    data = struct.pack('f'*len(param),*param)
    with open(save_file,'ab+') as f:
        f.write(data)


def linear_dump(module_dump:torch.nn.Module,save_file:String,bias_enable:bool=True):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    linear_weight = np.float32(module_dump.weight.detach().cpu().numpy())
    linear_weight = list(linear_weight.reshape(-1))
    print("linear_weight len:" + str(len(linear_weight)))
    data = struct.pack('f'*len(linear_weight),*linear_weight)
    with open(save_file,'ab+') as f:
        f.write(data)

    if(bias_enable):
        linear_bias = np.float32(module_dump.bias.detach().cpu().numpy())
        linear_bias = list(linear_bias.reshape(-1))
        print("linear_bias len:" + str(len(linear_bias)))
        data = struct.pack('f'*len(linear_bias),*linear_bias)
        with open(save_file,'ab+') as f:
            f.write(data)

def conv1d_dump(module_dump:torch.nn.Module,save_file:String):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    conv1d_weight = np.float32(module_dump.weight.detach().cpu().numpy())
    conv1d_weight = list(conv1d_weight.reshape(-1))
    print("conv1d_weight len:" + str(len(conv1d_weight)))
    data = struct.pack('f'*len(conv1d_weight),*conv1d_weight)
    with open(save_file,'ab+') as f:
        f.write(data)

    conv1d_bias = np.float32(module_dump.bias.detach().cpu().numpy())
    conv1d_bias = list(conv1d_bias.reshape(-1))
    print("conv1d_bias len:" + str(len(conv1d_bias)))
    data = struct.pack('f'*len(conv1d_bias),*conv1d_bias)
    with open(save_file,'ab+') as f:
        f.write(data)

def batchnorm1d_dump(module_dump:torch.nn.Module,save_file:String):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    norm_weight = np.float32(module_dump.weight.detach().cpu().numpy())
    norm_weight = list(norm_weight.reshape(-1))
    print("norm_weight len:" + str(len(norm_weight)))
    data = struct.pack('f'*len(norm_weight),*norm_weight)
    with open(save_file,'ab+') as f:
        f.write(data)

    norm_bias = np.float32(module_dump.bias.detach().cpu().numpy())
    norm_bias = list(norm_bias.reshape(-1))
    print("norm_bias len:" + str(len(norm_bias)))
    data = struct.pack('f'*len(norm_bias),*norm_bias)
    with open(save_file,'ab+') as f:
        f.write(data)

    norm_running_mean = np.float32(module_dump.running_mean.detach().cpu().numpy())
    norm_running_mean = list(norm_running_mean.reshape(-1))
    print("norm_running_mean len:" + str(len(norm_running_mean)))
    data = struct.pack('f'*len(norm_running_mean),*norm_running_mean)
    with open(save_file,'ab+') as f:
        f.write(data)

    norm_running_var = np.float32(module_dump.running_var.detach().cpu().numpy())
    norm_running_var = list(norm_running_var.reshape(-1))
    print("norm_running_var len:" + str(len(norm_running_var)))
    data = struct.pack('f'*len(norm_running_var),*norm_running_var)
    with open(save_file,'ab+') as f:
        f.write(data)

def prelu_dump(module_dump:torch.nn.Module,save_file:String):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    prelu_weight = np.float32(module_dump.weight.detach().cpu().numpy())
    prelu_weight = list(prelu_weight.reshape(-1))
    print("prelu_weight len:" + str(len(prelu_weight)))
    data = struct.pack('f'*len(prelu_weight),*prelu_weight)
    with open(save_file,'ab+') as f:
        f.write(data)

def lstm_dump(module_dump:torch.nn.Module,save_file:String):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    weight_ih_l0 = np.float32(module_dump.weight_ih_l0.detach().cpu().numpy())
    weight_ih_l0 = list(weight_ih_l0.reshape(-1))
    print("weight_ih_l0 len:" + str(len(weight_ih_l0)))
    data = struct.pack('f'*len(weight_ih_l0),*weight_ih_l0)
    with open(save_file,'ab+') as f:
        f.write(data)

    weight_hh_l0 = np.float32(module_dump.weight_hh_l0.detach().cpu().numpy())
    weight_hh_l0 = list(weight_hh_l0.reshape(-1))
    print("weight_hh_l0 len:" + str(len(weight_hh_l0)))
    data = struct.pack('f'*len(weight_hh_l0),*weight_hh_l0)
    with open(save_file,'ab+') as f:
        f.write(data)

    bias_h = module_dump.bias_ih_l0 + module_dump.bias_hh_l0
    bias_h = np.float32(bias_h.detach().cpu().numpy())
    bias_h = list(bias_h.reshape(-1))
    print("bias_hh_l0 len:" + str(len(bias_h)))
    data = struct.pack('f'*len(bias_h),*bias_h)
    with open(save_file,'ab+') as f:
        f.write(data)

def gru_dump(module_dump:torch.nn.Module,save_file:String):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    weight_ih_l0 = np.float32(module_dump.weight_ih_l0.detach().cpu().numpy())
    weight_ih_l0 = list(weight_ih_l0.reshape(-1))
    print("weight_ih_l0 len:" + str(len(weight_ih_l0)))
    data = struct.pack('f'*len(weight_ih_l0),*weight_ih_l0)
    with open(save_file,'ab+') as f:
        f.write(data)

    weight_hh_l0 = np.float32(module_dump.weight_hh_l0.detach().cpu().numpy())
    weight_hh_l0 = list(weight_hh_l0.reshape(-1))
    print("weight_hh_l0 len:" + str(len(weight_hh_l0)))
    data = struct.pack('f'*len(weight_hh_l0),*weight_hh_l0)
    with open(save_file,'ab+') as f:
        f.write(data)

    hidden_size = int(module_dump.bias_ih_l0.shape[0]/3)

    bias_h_0 = module_dump.bias_ih_l0[0:hidden_size*2] + module_dump.bias_hh_l0[0:hidden_size*2]
    bias_h_0 = np.float32(bias_h_0.detach().cpu().numpy())
    bias_h_0 = list(bias_h_0.reshape(-1))
    print("bias_h_0 len:" + str(len(bias_h_0)))
    data = struct.pack('f'*len(bias_h_0),*bias_h_0)
    with open(save_file,'ab+') as f:
        f.write(data)

    bias_h_1 = module_dump.bias_ih_l0[hidden_size*2:]
    bias_h_1 = np.float32(bias_h_1.detach().cpu().numpy())
    bias_h_1 = list(bias_h_1.reshape(-1))
    print("bias_h_1 len:" + str(len(bias_h_1)))
    data = struct.pack('f'*len(bias_h_1),*bias_h_1)
    with open(save_file,'ab+') as f:
        f.write(data)

    bias_h_2 = module_dump.bias_hh_l0[hidden_size*2:]
    bias_h_2 = np.float32(bias_h_2.detach().cpu().numpy())
    bias_h_2 = list(bias_h_2.reshape(-1))
    print("bias_h_2 len:" + str(len(bias_h_2)))
    data = struct.pack('f'*len(bias_h_2),*bias_h_2)
    with open(save_file,'ab+') as f:
        f.write(data)

def conv2d_dump(module_dump:torch.nn.Module,save_file:String):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    conv2d_weight = np.float32(module_dump.weight.detach().cpu().numpy())
    conv2d_weight = list(conv2d_weight.reshape(-1))
    print("conv1d_weight len:" + str(len(conv2d_weight)))
    data = struct.pack('f'*len(conv2d_weight),*conv2d_weight)
    with open(save_file,'ab+') as f:
        f.write(data)

    conv2d_bias = np.float32(module_dump.bias.detach().cpu().numpy())
    conv2d_bias = list(conv2d_bias.reshape(-1))
    print("conv1d_bias len:" + str(len(conv2d_bias)))
    data = struct.pack('f'*len(conv2d_bias),*conv2d_bias)
    with open(save_file,'ab+') as f:
        f.write(data)

def layernorm_dump(module_dump:torch.nn.Module,save_file:String):
    if os.path.exists(save_file):
        os.remove(save_file)

    print("------dump "+ save_file+'------')

    layernorm_weight = np.float32(module_dump.weight.detach().cpu().numpy())
    layernorm_weight = list(layernorm_weight.reshape(-1))
    print("layernorm_weight len:" + str(len(layernorm_weight)))
    data = struct.pack('f'*len(layernorm_weight),*layernorm_weight)
    with open(save_file,'ab+') as f:
        f.write(data)

    layernorm_bias = np.float32(module_dump.bias.detach().cpu().numpy())
    layernorm_bias = list(layernorm_bias.reshape(-1))
    print("layernorm_bias len:" + str(len(layernorm_bias)))
    data = struct.pack('f'*len(layernorm_bias),*layernorm_bias)
    with open(save_file,'ab+') as f:
        f.write(data)