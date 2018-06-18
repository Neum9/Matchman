#include "GameScene.h"
#include "LuaUtil.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{

}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//执行lua脚本读取场景
	lua_State* pL = LuaUtil::openLuaFile("luaData/testData.lua");

	//获取地图
	//获取地图数量
	lua_getglobal(pL, "getMapsLen");
	lua_call(pL, 0, 1);
	m_mapNum = lua_tonumber(pL, -1);
	log("mapsNum = %d", m_mapNum);
	
	//获取地图名
	for (int i = 0;i < m_mapNum;i++)
	{
		lua_getglobal(pL, "maps");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* m = lua_tostring(pL, -1);
		m_mapNames.push_back(Value(m).asString());
	}
	//关闭lua脚本
	LuaUtil::closeLuaFile(pL);

	//根据地图名读取文件
	auto tileMap = TMXTiledMap::create(m_mapNames.at(m_mapOrder));
	this->addChild(tileMap);

	//添加玩家

	return true;
}
