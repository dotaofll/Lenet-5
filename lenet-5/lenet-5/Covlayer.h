#pragma once
#include"mat.h"
#include"mnist.h"
#include"utils.h"
#include<random>
#include<numeric>
#include<limits>
#define _multi_array true;

static const size_t AvePool = 0;
static const size_t MaxPool = 1;
static const size_t MinPool = 2;

using sz=size_t;

class convolitional_layer {
public:
	sz inputWidth;
	sz inputHeight;
	sz mapsize;
	bool isFullConnect;
	sz inChannels;
	sz outChannels;
#ifdef _multi_array
	Array<float, 4> mapdata{ 1,1,1,1 };
	Array<float, 4> dmapdata{ 1,1,1,1 };

	Array<float, 3> v{ 1,1,1 };
	Array<float, 3> y{ 1,1,1 };
	Array<float, 3> d{ 1,1,1 };
	Array<float, 1> basicData{1};
#endif // _multi_array
	Mat2D mat;
	//do not accept the default construct 
	convolitional_layer(sz _inputWidth , sz _inputHeight,
						sz _mapSize , sz _inChannels , sz _outChannels ) :
		inputHeight{ _inputHeight }, inputWidth{ _inputWidth }
		, mapsize{ _mapSize }, inChannels{ _inChannels },
		outChannels{ _outChannels }, isFullConnect{ true }
	{
		Array<float, 4> _t_mapdata{ inChannels,outChannels,mapsize,mapsize };
		this->mapdata.swap(std::move(_t_mapdata));
		std::mt19937 gen{};
		std::normal_distribution<float> d{ 0,1 };
		for (sz i = 0; i < this->inChannels; i++) {
			for (sz j = 0; j < this->outChannels; j++) {
				for (sz r = 0; r < this->mapsize; r++) {
					for (sz c = 0; c < this->mapsize; c++) {
						float randnum = (d(gen) / 
										 std::numeric_limits<float>::max() - .5) * 2.0f;
						this->mapdata.ref(i, j, r, c) = randnum * std::sqrt(6.0f / (float)(mapsize * mapsize * (inChannels + outChannels)));
					}
				}
			}
		}
		Array<float, 4> _t_dmapdata{ inChannels,outChannels,mapsize,mapsize };
		this->dmapdata.swap(std::move(_t_dmapdata));
		Array<float, 1> _t_basicdata{ outChannels };
		this->basicData.swap(std::move(_t_basicdata));
		
		sz outW = this->inputWidth - this->mapsize + 1;
		sz outH = this->inputHeight - this->mapsize + 1;
		Array<float, 3> _t_data{ outChannels,outH,outW };
		this->d.swap(std::move(_t_data));
		this->v.swap(std::move(_t_data));
		this->y.swap(std::move(_t_data));

	}
	void CovLayerConnect(bool* connectModel);
};