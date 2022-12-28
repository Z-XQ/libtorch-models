#ifndef MLP_H
#define MLP_H
#endif // MLP_H

#include <torch/torch.h>
#include <torch/script.h>

// 小模块：fc+bn+relu
class LinearBnReluImpl : public torch::nn::Module {
public:
    LinearBnReluImpl(int intput_features, int output_features);
    torch::Tensor forward(torch::Tensor x);
private:
    //layers
    torch::nn::Linear ln{ nullptr };  // 定义私有成员，先构造函数初始化，再在forward函数使用。
    torch::nn::BatchNorm1d bn{ nullptr };
};
TORCH_MODULE(LinearBnRelu);

// 实现LinearBnRelu
// 注册线性层、bn层
LinearBnReluImpl::LinearBnReluImpl(int in_features, int out_features) {
    ln = register_module("ln", torch::nn::Linear(torch::nn::LinearOptions(in_features, out_features)));
    // 注意bn操作时，训练时batch_size必须设置成大于1，否则没意义且会报错，测试时会屏蔽此操作
    bn = register_module("bn", torch::nn::BatchNorm1d(out_features));
}
// linear->relu->bn
torch::Tensor LinearBnReluImpl::forward(torch::Tensor x) {
    x = torch::relu(ln->forward(x));
    x = bn(x);
    return x;
}

class MLP : public torch::nn::Module {
public:
    MLP(int in_features, int out_features);  // 构造函数：输入特征维度，和输出特征维度
    torch::Tensor forward(torch::Tensor x);  // 推理函数
private:
    int mid_features[3] = { 32,64,128 };       // 中间层特征维度
    LinearBnRelu ln1{ nullptr };               // 3个(linear + bn + relu)
    LinearBnRelu ln2{ nullptr };
    LinearBnRelu ln3{ nullptr };
    torch::nn::Linear out_ln{ nullptr };       // 普通的linear层
};

MLP::MLP(int in_features, int out_features) {
    ln1 = LinearBnRelu(in_features, mid_features[0]);  // 初始化
    ln2 = LinearBnRelu(mid_features[0], mid_features[1]);
    ln3 = LinearBnRelu(mid_features[1], mid_features[2]);
    out_ln = torch::nn::Linear(mid_features[2], out_features);

    ln1 = register_module("ln1", ln1);  // 构造函数注册轮子
    ln2 = register_module("ln2", ln2);
    ln3 = register_module("ln3", ln3);
    out_ln = register_module("out_ln", out_ln);
}

torch::Tensor MLP::forward(torch::Tensor x) {
    x = ln1->forward(x);   // 逐个forward，因为每个都是module，有各自的forward函数。
    x = ln2->forward(x);   // 
    x = ln3->forward(x);
    x = out_ln->forward(x);
    return x;
}