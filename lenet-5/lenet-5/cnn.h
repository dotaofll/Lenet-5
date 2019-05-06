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
	Array<float, 2> wdata{1,1};
	Array<float, 1> basicdata{1};
	Array<float, 1> v{1}; // 进入激活函数的输入值
	Array<float, 1> y{1}; // 激活函数后神经元的输出
	Array<float, 1> d{1}; // 网络的局部梯度,δ值
#endif //_multi_array

	bool isFullConnect; //是否为全连接
	nn_layer(sz _inputNum, sz _outputNum) :inputNum{ _inputNum }, outputNum{ _outputNum }
	{
		Array<float, 1> _t_bacdata{ outputNum };
		this->basicdata.swap(std::move(_t_bacdata));
		this->v.swap(std::move(_t_bacdata));
		this->y.swap(std::move(_t_bacdata));
		this->d.swap(std::move(_t_bacdata));
		Array<float, 2> _t_wdata{ outputNum,inputNum };
		this->wdata.swap(std::move(_t_wdata));
		std::mt19937 gen{};
		std::normal_distribution<float> d{ 0,1 };
		for (sz i = 0; i < this->outputNum; i++) {
			for (sz j = 0; j < this->inputNum; j++) {
				float randNum = (d(gen) / std::numeric_limits<float>::max() - .5) * 2;
				this->wdata.ref(i, j) = randNum * std::sqrt(6.0f / (float)(inputNum + outputNum));
			}
		}
	}
};
struct train_opts {
	int numepochs;
	float alpha;
};