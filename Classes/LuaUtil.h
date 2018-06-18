#pragma once
#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static class LuaUtil
{
public:
	static lua_State* openLuaFile(std::string file);
	static void closeLuaFile(lua_State* pL);
};