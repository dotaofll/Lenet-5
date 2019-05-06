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


	void cnnff(Array<float,2>&); // �����ǰ�򴫲�
	void cnnbp(Array<float,1>&); // ����ĺ��򴫲�
	void cnnapplygrads(train_opts& opts, Array<float,2>&);
	void cnnclear(); // ������vyd����
};

class nn_layer {
public:
	sz inputNum;
	sz outputNum;

#ifdef _multi_array
	Array<float, 2> wdata;
	Array<float, 1> basicdata;
	Array<float, 1> v; // ���뼤���������ֵ
	Array<float, 1> y; // ���������Ԫ�����
	Array<float, 1> d; // ����ľֲ��ݶ�,��ֵ
#endif //_multi_array

	bool isFullConnect; //�Ƿ�Ϊȫ����
	nn_layer();
};
struct train_opts {
	int numepochs;
	float alpha;
};