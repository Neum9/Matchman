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
	//定义按键按下
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//加入按下按键为已定义按键
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				m_playerManager.DoActionByKeyCode(i, keyCode);
				//左跑
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 1)
				{
					m_playerManager.changePlayerRunDirByID(i, -1);
				}
				//右跑
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager.changePlayerRunDirByID(i, 1);
				}
				m_playerManager.ReLoadActionByID(i);
				return;
			}
		}
	};
	//定义按键松开，当执行行走等动作时可以直接让玩家执行stand，其他不可结束时应该等待动作完成再执行stand
	listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//检测松开的按键为哪个player的
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				//之前左跑
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 1)
				{
					m_playerManager.changePlayerRunDirByID(i, 1);
				}
				//之前右跑
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager.changePlayerRunDirByID(i, -1);
				}
				m_playerManager.ReLoadActionByID(i);
				return;
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//添加定时器
	this->scheduleUpdate();

	return true;
}

bool GameScene::isTileCanbeStand(float x, float y)
{
	return true;
}

std::vector<std::string> GameScene::m_mapNames;

int GameScene::m_mapNum;

int GameScene::m_mapOrder;

void GameScene::update(float dt)
{
	if (m_playerManager.IsPlayersCollide())
	{
		log("player collide!");
	}
}
