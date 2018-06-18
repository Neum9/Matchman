#include "PlayerType.h"
#include "cocos2d.h"
#include "LuaUtil.h"
USING_NS_CC;

std::vector<std::string> PlayerType::m_playerTypes;

void PlayerType::LoadTypeFromLua()
{
	lua_State* pL = LuaUtil::openLuaFile("luaData/testData.lua");

	//获取玩家类型数量
	lua_getglobal(pL, "getPlayerTypeNum");
	lua_call(pL, 0, 1);

	int playerTypeNum = lua_tonumber(pL, -1);

	//读取玩家类型
	for (int i = 0;i < playerTypeNum;i++)
	{
		lua_getglobal(pL, "playerType");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* s = lua_tostring(pL, -1);
		m_playerTypes.push_back(Value(s).asString());
	}

	LuaUtil::closeLuaFile(pL);
}

std::string PlayerType::GetPlayerTypeByOrder(int order)
{
	return m_playerTypes.at(order);
}
