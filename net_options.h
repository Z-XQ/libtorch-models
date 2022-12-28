#pragma once
#include <torch/torch.h>
#include <torch/script.h>

// 设置卷积参数配置函数
inline torch::nn::Conv2dOptions conv2d_options(int64_t in_c, int64_t out_c, int64_t k_xsize, int64_t k_ysize, int64_t stride = 1, int64_t padding = 0)
{
	torch::nn::Conv2dOptions cur_conv2d_options = torch::nn::Conv2dOptions(in_c, out_c, { k_xsize,k_ysize });
	cur_conv2d_options.stride(stride);
	cur_conv2d_options.padding(padding);
	return cur_conv2d_options;
}

inline torch::nn::MaxPool2dOptions max_pool_options(int64_t k_xsize, int64_t k_ysize, int stride)
{
	torch::nn::MaxPool2dOptions cur_options = torch::nn::MaxPool2dOptions({ k_xsize , k_ysize });
	cur_options.stride(stride);
	return cur_options;
}

inline torch::nn::LinearOptions linear_options(int64_t in_c, int64_t out_c) {
	torch::nn::LinearOptions cur_options = torch::nn::LinearOptions(in_c, out_c);
	return cur_options;
}

inline torch::nn::Conv1dOptions conv1d_options(int64_t in_c, int64_t out_c, int64_t k_size, int64_t stride = 1, int64_t padding = 0)
{
	torch::nn::Conv1dOptions cur_options = torch::nn::Conv1dOptions(in_c, out_c, k_size);
	cur_options.stride(stride);
	cur_options.padding(padding);
	return cur_options;
}

inline torch::optim::AdamOptions Adam_options(double lr, double eps = 1e-05, double weight_decay=0.0)
{
	torch::optim::AdamOptions cur_options = torch::optim::AdamOptions(lr);
	cur_options.eps(eps).weight_decay(weight_decay);
	return cur_options;
}