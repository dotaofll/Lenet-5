#pragma once
#include"Covlayer.h"


class pooling_layer {
public:
	sz inputWidth;   //����ͼ��Ŀ�
	sz inputHeight;  //����ͼ��ĳ�
	sz mapSize;      //����ģ��Ĵ�С

	sz inChannels;   //����ͼ�����Ŀ
	sz outChannels;  //���ͼ�����Ŀ

	sz poolType;     //Pooling�ķ���
		
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