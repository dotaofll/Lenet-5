#pragma once
#include"Covlayer.h"


class pooling_layer {
public:
	sz inputWidth;   //输入图像的宽
	sz inputHeight;  //输入图像的长
	sz mapSize;      //特征模板的大小

	sz inChannels;   //输入图像的数目
	sz outChannels;  //输出图像的数目

	sz poolType;     //Pooling的方法
	float* basicData;   //偏置
#ifdef _multi_array
	Array<float, 3> y;
	Array<float, 3> d;
	float*** y; // 采样函数后神经元的输出,无激活函数
	float*** d; // 网络的局部梯度,δ值
#endif //_multi_array
	Mat2D mat;
	pooling_layer();
	void PoolLayerConnect(bool* connectModel);
};