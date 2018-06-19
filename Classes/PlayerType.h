#pragma once
#include <vector>
#include "cocos2d.h"

USING_NS_CC;

static class PlayerType
{
public:
	//从lua文件读取数据
	static void LoadTypeFromLua(const char* file);
	static std::string GetPlayerTypeByOrder(int order);
private:
	//存储玩家类型
	static std::vector<std::string> m_playerTypes;
};