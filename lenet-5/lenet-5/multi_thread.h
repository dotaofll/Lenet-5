#pragma once
#include <vector>
#include <thread>
#include <cassert>
using namespace std;

vector<int> divid(int fold, int max_num)
{
	assert(max_num >= fold);
	vector<int> res(fold, 0);
	for (int i = 1; i < fold; i++)
	{
		res[i] = (i - 1) * (max_num / fold) + (max_num / fold);
	}
	return res;
}
