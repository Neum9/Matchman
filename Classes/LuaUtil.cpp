#include "LuaUtil.h"

lua_State* LuaUtil::openLuaFile(std::string file)
{
	//执行lua脚本读取
	lua_State* pL = lua_open();
	luaopen_base(pL);
	luaopen_math(pL);
	luaopen_string(pL);

	//执行lua脚本，返回0代表成功
	int err = luaL_dofile(pL, file.c_str());
	log("open = %d", err);
	//重置栈顶索引
	lua_settop(pL, 0);

	return pL;
}

void LuaUtil::closeLuaFile(lua_State* pL)
{
	lua_close(pL);
}
