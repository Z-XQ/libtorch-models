#ifndef MLP_H
#define MLP_H
#endif // MLP_H

#include <torch/torch.h>
#include <torch/script.h>

// Сģ�飺fc+bn+relu
class LinearBnReluImpl : public torch::nn::Module {
public:
    LinearBnReluImpl(int intput_features, int output_features);
    torch::Tensor forward(torch::Tensor x);
private:
    //layers
    torch::nn::Linear ln{ nullptr };  // ����˽�г�Ա���ȹ��캯����ʼ��������forward����ʹ�á�
    torch::nn::BatchNorm1d bn{ nullptr };
};
TORCH_MODULE(LinearBnRelu);

// ʵ��LinearBnRelu
// ע�����Բ㡢bn��
LinearBnReluImpl::LinearBnReluImpl(int in_features, int out_features) {
    ln = register_module("ln", torch::nn::Linear(torch::nn::LinearOptions(in_features, out_features)));
    // ע��bn����ʱ��ѵ��ʱbatch_size�������óɴ���1������û�����һᱨ������ʱ�����δ˲���
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
    MLP(int in_features, int out_features);  // ���캯������������ά�ȣ����������ά��
    torch::Tensor forward(torch::Tensor x);  // ������
private:
    int mid_features[3] = { 32,64,128 };       // �м������ά��
    LinearBnRelu ln1{ nullptr };               // 3��(linear + bn + relu)
    LinearBnRelu ln2{ nullptr };
    LinearBnRelu ln3{ nullptr };
    torch::nn::Linear out_ln{ nullptr };       // ��ͨ��linear��
};

MLP::MLP(int in_features, int out_features) {
    ln1 = LinearBnRelu(in_features, mid_features[0]);  // ��ʼ��
    ln2 = LinearBnRelu(mid_features[0], mid_features[1]);
    ln3 = LinearBnRelu(mid_features[1], mid_features[2]);
    out_ln = torch::nn::Linear(mid_features[2], out_features);

    ln1 = register_module("ln1", ln1);  // ���캯��ע������
    ln2 = register_module("ln2", ln2);
    ln3 = register_module("ln3", ln3);
    out_ln = register_module("out_ln", out_ln);
}

torch::Tensor MLP::forward(torch::Tensor x) {
    x = ln1->forward(x);   // ���forward����Ϊÿ������module���и��Ե�forward������
    x = ln2->forward(x);   // 
    x = ln3->forward(x);
    x = out_ln->forward(x);
    return x;
}