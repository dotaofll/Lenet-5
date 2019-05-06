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
		
#ifdef _multi_array
	Array<float, 1> basicData{1};
	Array<float, 3> y{1,1,1};
	Array<float, 3> d{1,1,1};
#endif //_multi_array
	Mat2D mat;
	pooling_layer(int _inputWidth, int _inputHeight, 
				  int _mapSize, int _inChannels,
				  int _outChannels, int _poolType) :
		inputHeight{ _inputHeight }, inputWidth{ _inputWidth }
		, mapSize{ _mapSize }, inChannels{ _inChannels },
		outChannels{ _outChannels }, poolType{_poolType}
	{
		Array<float, 1> _t_bacdata{ outChannels };
		this->basicData.swap(std::move(_t_bacdata));
		sz outW = inputWidth / mapSize;
		sz outH = inputHeight / mapSize;
		Array<float, 3> _t_data{ outChannels,outH,outW };
		this->d.swap(std::move(_t_data));
		this->y.swap(std::move(_t_data));
	}
	void PoolLayerConnect(bool* connectModel);
};