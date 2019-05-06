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
	float* basicData;   //ƫ��
#ifdef _multi_array
	Array<float, 3> y;
	Array<float, 3> d;
	float*** y; // ������������Ԫ�����,�޼����
	float*** d; // ����ľֲ��ݶ�,��ֵ
#endif //_multi_array
	Mat2D mat;
	pooling_layer();
	void PoolLayerConnect(bool* connectModel);
};