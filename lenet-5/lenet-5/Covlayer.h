#pragma once
#include"mat.h"
#include"mnist.h"
#include"utils.h"

static const size_t AvePool = 0;
static const size_t MaxPool = 1;
static const size_t MinPool = 2;

using sz=size_t;

class convolitional_layer {
public:
	sz inputWidth;
	sz inputHeight;
	sz mapsize;

	sz inChannels;
	sz outChannels;
#ifdef _multi_array
	Array<float, 4> mapdata{ 1,1,1,1 };
	Array<float, 4> dmapdata{ 1,1,1,1 };

	Array<float, 3> v{ 1,1,1 };
	Array<float, 3> y{ 1,1,1 };
	Array<float, 3> d{ 1,1,1 };
#endif // _multi_array
	Mat2D mat;
	convolitional_layer();
	void CovLayerConnect(bool* connectModel);
};