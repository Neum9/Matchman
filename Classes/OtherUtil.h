#pragma once
#include <vector>
#include <algorithm>
#include <iterator>

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
	std::vector<T>::iterator ite = find(vec.begin(), vec.end(), e);
	if (ite != vec.end())
		return true;
	return false;
}

//UI信息
class UIMessage : public Ref
{
public:
	int id;
	int health;
	int power;
};