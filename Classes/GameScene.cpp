#include "GameScene.h"
#include "LuaUtil.h"
#include "OtherUtil.h"
#include "PlayerManager.h"



GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

Scene* GameScene::createScene(PlayerManager playerManage)
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	layer->m_playerManager = playerManage;
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

	//注册键盘监听函数
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//加入按下按键为已定义按键
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				m_playerManager.DoActionByKeyCode(i, keyCode);
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//添加玩家

	return true;
}