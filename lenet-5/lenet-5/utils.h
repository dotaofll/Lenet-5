#pragma once

#ifdef __PPL
#include <ppl.h>
#include <mutex>
#include <mnist.h>
std::mutex g_pages_mutex;
void load_mnist_data(const string &&file_name, MinstImg &Img){
	//images[i] = tp;
	//auto task1([&]() {
	//	for (int i = 0; i < 15000; i++) {
	//		//std::lock_guard lg{ g_pages_mutex };
	//		for (int r = 0; r < n_rows; r++)
	//		{
	//			for (int c = 0; c < n_cols; c++)
	//			{
	//				tp.push_back(buf[c]);
	//			}
	//		}
	//		images[i] = tp;
	//	}
	//		   });
	//auto task2([&]() {
	//	for (int i = 15001; i < 30000; i++) {
	//		//std::lock_guard lg{ g_pages_mutex };
	//		for (int r = 0; r < n_rows; r++)
	//		{
	//			for (int c = 0; c < n_cols; c++)
	//			{
	//				tp.push_back(buf[c]);
	//			}
	//		}
	//		images[i] = tp;
	//	}
	//		   });
	//auto task3([&]() {
	//	for (int i = 30001; i < 45000; i++) {
	//		//std::lock_guard lg{ g_pages_mutex };
	//		for (int r = 0; r < n_rows; r++)
	//		{
	//			for (int c = 0; c < n_cols; c++)
	//			{
	//				tp.push_back(buf[c]);
	//			}
	//		}
	//		images[i] = tp;
	//	}
	//		   });
	//auto task4([&]() {
	//	for (int i = 45001; i < 60000; i++) {
	//		//std::lock_guard lg{ g_pages_mutex };
	//		for (int r = 0; r < n_rows; r++)
	//		{
	//			for (int c = 0; c < n_cols; c++)
	//			{
	//				tp.push_back(buf[c]);
	//			}
	//		}
	//		images[i] = tp;
	//	}
	//		   });
	//thread t1{ task1 };
	//thread t2{ task2 };
	//thread t3{ task3 };
	//thread t4{ task4 };
	//t1.join(); t2.join(); t3.join(); t4.join();
};
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "mnist.h"
using namespace std;

using vvd = vector<vector<double>>;

int translateEndian_32(int i)
{
	return ((i & 0x000000FF) << 24 | (i & 0x0000FF00) << 8 | (i & 0x00FF0000) >> 8 | (i & 0xFF000000) >> 24);
}

int load_mnist_data(const string file_name, MnistImg &ImgPtr)
{
	shared_ptr<ifstream> in{new ifstream};
	in->open(file_name, ios::binary);
	if (!in->is_open())
	{
		throw std::runtime_error("can not open file");
	}
	int magic_number{0};
	int number_of_images{0};
	int n_rows{0}, n_cols{0};

	in->read((char *)&magic_number, sizeof magic_number);
	magic_number = translateEndian_32(magic_number);

	in->read((char *)&number_of_images, sizeof number_of_images);
	number_of_images = translateEndian_32(number_of_images);

	in->read(reinterpret_cast<char *>(&n_rows), sizeof n_rows);
	n_rows = translateEndian_32(n_rows);

	in->read(reinterpret_cast<char *>(&n_cols), sizeof n_cols);
	n_cols = translateEndian_32(n_cols);

	auto pk(in->tellg());
	vector<char> buf(in->seekg(pk, std::ios::end).tellg());
	in->seekg(pk, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(buf.size()));

	/*concurrency::parallel_for(1, number_of_images+1, [&] (int idx) {
		vector<double> tp;
		for (int r = 0; r < n_rows; r++)
		{
			for (int c = 0; c < n_cols; c++)
			{
				tp.push_back(buf[c]);
			}
			images.push_back(tp);
		}
							  });*/
	vector<vector<float>> tp(32, vector<float>(32, 0.0));
	for (int i = 0; i < number_of_images; i++)
	{
		for (int r = 0; r < n_rows; r++)
		{
			for (int c = 0; c < n_cols; c++)
			{
				tp[r][c] = static_cast<float>(buf[c] / 255);
			}
		}
		ImgPtr.ImgPtr[i].Imgdata = tp;
	}
	buf.shrink_to_fit();
	tp.shrink_to_fit();
	return 1;
}

int load_mnist_label(const string &&file_name, MnistLable &LabelPtr)
{
	shared_ptr<ifstream> in{new ifstream};
	in->open(file_name, ios::binary);
	if (!in->is_open())
	{
		throw std::runtime_error("can not open file");
	}
	int magic_number{0};
	int number_of_labels{0};
	int label_length{10};

	in->read((char *)&magic_number, sizeof magic_number);
	magic_number = translateEndian_32(magic_number);

	in->read((char *)&number_of_labels, sizeof number_of_labels);
	number_of_labels = translateEndian_32(number_of_labels);

	for (int i = 0; i < number_of_labels; i++)
	{
		unsigned char temp{0};
		in->read((char *)&temp, sizeof temp);
		LabelPtr.LabelPtr[i].Labeldata[(int)temp] = 1.0;
	}
	return 1;
}

int save_Img(MnistImg &ImgPtr, const string &&file)
{
	int img_number = ImgPtr.ImgNum;
	for (int i = 0; i < img_number; i++)
	{
		const string filename = file + to_string(i) + "gray";
		shared_ptr<ofstream> out{make_shared<ofstream>()};
		out->open(filename, ios::binary);
		if (!out->is_open())
		{
			throw runtime_error("can not open file");
		}
		for (int j = 0; j < ImgPtr.ImgPtr[i].height; j++)
		{
			auto temp = ImgPtr.ImgPtr[i].Imgdata[j];
			for (auto num : temp)
			{
				out->write(reinterpret_cast<char *>(&num), sizeof num);
			}
		}
	}
}
