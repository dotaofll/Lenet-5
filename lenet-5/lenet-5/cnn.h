#pragma once
#include"PoolLayer.h"

class cnn_network {
public:

	sz layerNum;


	convolitional_layer* c1;
	pooling_layer* s2;
	convolitional_layer* c3;
	pooling_layer* s4;
	nn_layer* o5;


	void cnnff(Array<float,2>&); // 网络的前向传播
	void cnnbp(Array<float,1>&); // 网络的后向传播
	void cnnapplygrads(train_opts& opts, Array<float,2>&);
	void cnnclear(); // 将数据vyd清零
};

class nn_layer {
public:
	sz inputNum;
	sz outputNum;

#ifdef _multi_array
	Array<float, 2> wdata;
	Array<float, 1> basicdata;
	Array<float, 1> v; // 进入激活函数的输入值
	Array<float, 1> y; // 激活函数后神经元的输出
	Array<float, 1> d; // 网络的局部梯度,δ值
#endif //_multi_array

	bool isFullConnect; //是否为全连接
	nn_layer();
};
struct train_opts {
	int numepochs;
	float alpha;
};