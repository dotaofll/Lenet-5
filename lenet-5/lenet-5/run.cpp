#include "utils.h"
#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>
#include "multi_thread.h"
#include "mnist.h"
#include "mat.h"
using namespace std;
using namespace chrono;
int main()
{
	//MnistImg res;
	//MnistLable res_l;
	//auto start = system_clock::now();
	//load_mnist_data("train-images.idx3-ubyte", res);
	//load_mnist_label("train-labels.idx1-ubyte", res_l);
	////save_Img(res, "image");
	//auto end = system_clock::now();
	//auto duration = duration_cast<microseconds>(end - start);
	//cout << "almost cost"
	//	<< double(duration.count()) * microseconds::period::num / microseconds::period::den
	//	<< "seconds" << endl;
	//shared_ptr<float**> ptr{ make_shared<>(float**) };
	Mat2D src{6, 6};
	Mat2D map{4, 4};
	mt19937 gen{};
	normal_distribution<> d(5);
	for (auto &fv : src.mat)
	{
		for (auto &fn : fv)
		{
			fn = (float)d(gen);
		}
	}
	for (auto &fv : map.mat)
	{
		for (auto &fn : fv)
		{
			fn = (float)d(gen);
		}
	}
	vvf ret = src.cov(map, src, 0);
	cout << d(gen);
	return 0;
}