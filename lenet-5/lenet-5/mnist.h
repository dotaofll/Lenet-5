#pragma once

#include <string>
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <vector>
#include <memory>

using vf = std::vector<float>;
using sz = std::size_t;

class MnistImg;
class MnistLable;

class MnistImg_meta
{

private:
  friend class MnistImg;

public:
  sz wight;
  sz height;
  MnistImg_meta(sz _wight = 32, sz _height = 32) : wight{_wight}, height{_height}, Imgdata(wight, std::vector<float>(height, 0.0)) {}
  ~MnistImg_meta() = default;
  MnistImg_meta(MnistImg_meta &) = delete;
  MnistImg_meta &operator=(MnistImg_meta &) = delete;
  std::vector<vf> Imgdata;
};

class MnistImg
{
public:
  sz ImgNum;
  std::vector<MnistImg_meta> ImgPtr;

  MnistImg(sz num = 60000) : ImgNum{num}, ImgPtr(ImgNum) {}
  sz size() { return this->ImgNum; }
};

class MnistLable_meta
{
private:
  friend class MnistLable;
  sz length;

public:
  vf Labeldata;
  MnistLable_meta() : length{10}, Labeldata(10, 0.0) {}
  MnistLable_meta(MnistLable_meta &) = delete;
  MnistLable_meta &operator=(MnistLable_meta &) = delete;
};

class MnistLable
{
private:
  sz LabelNum;

public:
  std::vector<MnistLable_meta> LabelPtr;
  MnistLable(sz num = 60000) : LabelNum{num}, LabelPtr(LabelNum) {}
  sz size() { return LabelNum; }
};
