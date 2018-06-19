#pragma once
#include <vector>


static class OtherUtil
{
public:
	//vector中是否包含该元素
	template <class T>
	static bool isContain(std::vector<T> vec, T e);
};

template <typename T>
bool OtherUtil::isContain(std::vector<T> vec, T e)
{
	for (T v : vec)
	{
		if (v == e)
			return true;
	}
	return false;
}