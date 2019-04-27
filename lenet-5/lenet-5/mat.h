#pragma once
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <memory>

using vvf = std::vector<std::vector<float>>;

const size_t full = 0;
const size_t same = 1;
const size_t valid = 2;

class Mat2D
{
  public:
	size_t row;
	size_t col;
	Mat2D(size_t _row = 0, size_t _col = 0) : row{_row}, col{_col},
											  mat(row, std::vector<float>(col)) {}
	~Mat2D() { mat.~vector(); }
	vvf mat;
	vvf rotate180();
	vvf correlation(Mat2D &, Mat2D &, size_t);
	vvf cov(Mat2D &, Mat2D &, size_t);
	vvf UpSample(Mat2D &, size_t, size_t);
	vvf matEdgeExpand(Mat2D &, size_t, size_t);
	vvf matEdgeShrink(Mat2D &, size_t, size_t);
	void addmat(vvf &, Mat2D &, Mat2D &);
	void savemat(Mat2D &, std::string);
	void multifactor(vvf &, Mat2D &, float);
	float summat(Mat2D &);
};