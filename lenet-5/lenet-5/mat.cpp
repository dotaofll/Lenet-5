#include "mat.h"
#include <algorithm>
#define ms2M std::make_shared<Mat2D>
using sp2M = std::shared_ptr<Mat2D>;

vvf Mat2D::rotate180()
{
	size_t outsizeC = this->col;
	size_t outsizeR = this->row;
	sp2M temp{ms2M(outsizeC, outsizeR)};
	for (int r = 0; r < outsizeR; r++)
	{
		for (int c = 0; c < outsizeC; c++)
		{
			temp->mat[r][c] = this->mat[outsizeR - r - 1][outsizeC - c - 1];
		}
	}
	this->mat = temp->mat;
	return this->mat;
}


vvf Mat2D::correlation(Mat2D &map, size_t type = full)
{ //this
	int halfmapsizew{0};
	int halfmapsizeh{0};
	if (map.row % 2 == 0 && map.col % 2 == 0)
	{
		halfmapsizeh = map.row / 2;
		halfmapsizew = map.col / 2;
		//halfmapsizeh = this->mat.row / 2;
		//halfmapsizew = this->mat.col / 2;
	}
	else
	{
		halfmapsizeh = (map.row - 1) / 2;
		halfmapsizew = (map.col - 1) / 2;
		//halfmapsizeh = (this->mat.row-1) / 2;
		//halfmapsizew = (this->mat.col-1) / 2;
	}
	size_t outsizew{this->col + map.col - 1};
	size_t outsizeh{this->row + map.row - 1};
	//size_t outsizew{input.col+this->mat.col-1};
	//size_t outsizeh{input.col+this->mat.row-1};

	vvf outputData(outsizeh, std::vector<float>(outsizew, 0.0));

	vvf exInput = matEdgeExpand(*this, map.col - 1, map.row - 1);

	for (int i = 0; i < outsizeh; i++)
	{
		for (int j = 0; j < outsizew; j++)
		{
			for (int r = 0; r < map.row; r++)
			{
				for (int c = 0; c < map.col; c++)
				{
					outputData[i][j] = outputData[i][j] +
									   map.mat[r][c] * exInput[i + r][j + c];
				}
			}
		}
	}
	exInput.~vector();
	Mat2D out(outsizeh, outsizew);
	out.mat = outputData;
	vvf sameres = matEdgeShrink(out, halfmapsizew, halfmapsizeh);
	vvf validers;
	if (map.row % 2 == 0 && map.col % 2 == 0)
	{
		validers = matEdgeShrink(out, halfmapsizew * 2 - 1, halfmapsizeh * 2 - 1);
	}
	else
	{
		validers = matEdgeShrink(out, halfmapsizew * 2, halfmapsizeh * 2);
	}
	switch (type)
	{
	case full:
		return outputData;
	case same:
		return sameres;
	case valid:
		return validers;
	default:
		return outputData;
	}
}

vvf Mat2D::cov(Mat2D &map, size_t type = full)
{
	vvf flipmap = rotate180();
	//map.mat = flipmap;
	vvf res = correlation(map, type);
	return res;
}
// ����Ǿ�����ϲ�������ֵ�ڲ壩��upc��upr���ڲ屶��
vvf Mat2D::UpSample(Mat2D &Mat, size_t upc, size_t upr)
{
	size_t col{Mat.col};
	size_t row{Mat.row};
	vvf res(row * upr, std::vector<float>(col * upc));
	for (int j = 0; j < row * upr; j = j + upr)
	{
		for (int i = 0; i < col * upc; i = i + upc) // ��������
			for (int m = 0; m < upc; m++)
				res[j][i + m] = mat[j / upr][i / upc];

		for (int n = 1; n < upr; n++) //  �ߵ�����
			for (int i = 0; i < col * upc; i++)
				res[j + n][i] = res[j][i];
	}
	return res;
}

vvf Mat2D::matEdgeExpand(Mat2D &Mat, size_t addc, size_t addr)
{
	size_t col{Mat.col};
	size_t row{Mat.row};
	vvf res(row + 2 * addr, std::vector<float>(col + 2 * addc));
	for (int j = 0; j < row + 2 * addr; j++)
	{
		for (int i = 0; i < col + 2 * addc; i++)
		{
			if (j < addr || i < addc || j >= (row + addr) || i >= (col + addc))
				res[j][i] = (float)0.0;
			else
				res[j][i] = mat[j - addr][i - addc]; // ����ԭ����������
		}
	}
	return res;
}

vvf Mat2D::matEdgeShrink(Mat2D &Mat, size_t shrinkc, size_t shrinkr)
{
	size_t c{Mat.col};
	size_t r{Mat.row};
	vvf res(r - 2 * shrinkr, std::vector<float>(c - 2 * shrinkc));
	for (int j = 0; j < r; j++)
	{
		for (int i = 0; i < c; i++)
		{
			if (j >= shrinkr && i >= shrinkc && j < (r - shrinkr) && i < (c - shrinkc))
				res[j - shrinkr][i - shrinkc] = Mat.mat[j][i]; // ����ԭ����������
		}
	}
	return res;
}

void Mat2D::addmat(vvf &res, Mat2D &Mat2)
{
	if (this->col != Mat2.col || this->row != Mat2.row)
	{
		throw std::runtime_error("Size is not same");
	}
	for (int i = 0; i < this->row; i++)
	{
		for (int j = 0; j < Mat2.col; j++)
		{
			res[i][j] = this->mat[i][j] + Mat2.mat[i][j];
		}
	}
}

void Mat2D::multifactor(vvf &res, float factor)
{
	/*transform(this->mat.begin(), this->mat.end(), back_inserter(res),
	 [&](std::vector<float> f) { std::for_each(f.begin(),end(f),); });*/
}

float Mat2D::summat()
{
	float sum = 0.0f;
	for (auto &fv : this->mat)
	{
		sum = std::accumulate(fv.begin(), fv.end(), 0.f);
	}
	return sum;
}
