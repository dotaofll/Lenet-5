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
#include <cassert>
#include "mnist.h"


using vvd = std::vector<std::vector<double>>;


namespace io {
	int translateEndian_32(int i)
	{
		return ((i & 0x000000FF) << 24 | (i & 0x0000FF00) << 8 | (i & 0x00FF0000) >> 8 | (i & 0xFF000000) >> 24);
	}

	int load_mnist_data(const std::string file_name, MnistImg& ImgPtr)
	{
		std::shared_ptr<std::ifstream> in{ new std::ifstream };
		in->open(file_name, std::ios::binary);
		if (!in->is_open())
		{
			throw std::runtime_error("can not open file");
		}
		int magic_number{ 0 };
		int number_of_images{ 0 };
		int n_rows{ 0 }, n_cols{ 0 };

		in->read((char*)& magic_number, sizeof magic_number);
		magic_number = translateEndian_32(magic_number);

		in->read((char*)& number_of_images, sizeof number_of_images);
		number_of_images = translateEndian_32(number_of_images);

		in->read(reinterpret_cast<char*>(&n_rows), sizeof n_rows);
		n_rows = translateEndian_32(n_rows);

		in->read(reinterpret_cast<char*>(&n_cols), sizeof n_cols);
		n_cols = translateEndian_32(n_cols);

		auto pk(in->tellg());
		std::vector<char> buf(in->seekg(pk, std::ios::end).tellg());
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
		std::vector<std::vector<float>> tp(32, std::vector<float>(32, 0.0));
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

	int load_mnist_label(const std::string&& file_name, MnistLable& LabelPtr)
	{
		std::shared_ptr<std::ifstream> in{ new std::ifstream };
		in->open(file_name, std::ios::binary);
		if (!in->is_open())
		{
			throw std::runtime_error("can not open file");
		}
		int magic_number{ 0 };
		int number_of_labels{ 0 };
		int label_length{ 10 };

		in->read((char*)& magic_number, sizeof magic_number);
		magic_number = translateEndian_32(magic_number);

		in->read((char*)& number_of_labels, sizeof number_of_labels);
		number_of_labels = translateEndian_32(number_of_labels);

		for (int i = 0; i < number_of_labels; i++)
		{
			unsigned char temp{ 0 };
			in->read((char*)& temp, sizeof temp);
			LabelPtr.LabelPtr[i].Labeldata[(int)temp] = 1.0;
		}
		return 1;
	}

	int save_Img(MnistImg& ImgPtr, const std::string&& file)
	{
		int img_number = ImgPtr.ImgNum;
		for (int i = 0; i < img_number; i++)
		{
			const std::string filename = file + std::to_string(i) + "gray";
			std::shared_ptr<std::ofstream> out{ std::make_shared<std::ofstream>() };
			out->open(filename, std::ios::binary);
			if (!out->is_open())
			{
				throw std::runtime_error("can not open file");
			}
			for (int j = 0; j < ImgPtr.ImgPtr[i].height; j++)
			{
				auto temp = ImgPtr.ImgPtr[i].Imgdata[j];
				for (auto num : temp)
				{
					out->write(reinterpret_cast<char*>(&num), sizeof num);
				}
			}
		}
	}
};

//template<typename T>
//class num_iterator {
//public:
//	T val;
//	num_iterator
//};

namespace Arrayhelper {
	inline int _fill(int* eachsize, int a) {
		assert(a > 0);
		*eachsize = a;
		return a;
	}
	template<typename T,typename... Args>
	inline int _fill(int* eachsize, T a, Args... args) {
		static_assert(std::is_integral<T>::value, "Dim must be integral");
		return _fill(eachsize, static_cast<int>(a)) 
			* _fill(eachsize + 1, args...);
	}
	inline int getIndex(int* size, int* weight, int idx) {
		assert(idx >= 0 && idx < *size);
		return idx * (*weight);
	}
	template<typename T,typename... Args>
	inline int getIndex(int* size, int* weight,
						T idx, Args... args) {
		static_assert(std::is_integral<T>::value, "Dim must be integral");
		int idx0 = getIndex(size, weight, static_cast<int>(idx));
		int idx1 = getIndex(size + 1, weight + 1, args...);
		return idx0 + idx1;
	}
};

template<typename T,int dim>
class Array final {
public:
	Array() :_values(nullptr), _totalSize{ 0 }, _dimInfo{0,0}{}
	template<typename... Args>
	Array(Args... args) {
		static_assert(sizeof...(Args) == dim, "args lenth is not match the dim");
		static_assert(0 != dim, "dim comes to be 0");

		this->_totalSize = Arrayhelper::_fill(_dimInfo.sizes, args...);
		assert(this->_totalSize != 0);
		_values = new T[this->_totalSize];
		_dimInfo.weight[dim - 1] = 1;
		for (int i = dim - 1; i >= 1; i--) {
			_dimInfo.weight[i - 1] = _dimInfo.weight[i] * _dimInfo.sizes[i];
		}
	}
	~Array() {
		if (this->_values) {
			delete[] this->_values;
			this->_values = nullptr;
		}
	}
	Array(Array&& rhs) {
		_values = rhs._values;
		_dimInfo = rhs._dimInfo;
		_totalSize = rhs._totalSize;
		rhs._values = nullptr;
	}
	void swap(Array&& rhs) {
		_values = rhs._values;
		_dimInfo = rhs._dimInfo;
		_totalSize = rhs._totalSize;
		rhs._values = nullptr;
	}
	template<typename... Args>
	T& ref(Args... args) {
		static_assert(sizeof...(Args) == dim, "");
		assert(this->_values);
		int idx = Arrayhelper::getIndex(_dimInfo.sizes, _dimInfo.weight, args...);
		assert(idx < this->_totalSize);
		return this->_values[idx];
	}
	template <typename... Args>
	const T& ref(Args... args) const {
		return const_cast<Array<T, dim>*>(this)->ref(args...);
	}
	Array& operator=(const Array& rhs) = delete;
	Array(const Array& rhs) = delete;

private:
	T* _values;
	int _totalSize;
	struct {
		int weight[dim];
		int sizes[dim];
	} _dimInfo;
};
