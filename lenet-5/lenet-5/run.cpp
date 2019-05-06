#include "utils.h"
#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>
#include <chrono>
#include "multi_thread.h"
#include "mnist.h"
#include "mat.h"
using namespace std;
//using namespace chrono;
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
	vector<float> res;
	Array<float, 2> a1(2,2);
	Array<float, 2> a0(move(a1));
	
	return 0;
}