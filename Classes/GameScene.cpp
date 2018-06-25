#include "GameScene.h"
#include "LuaUtil.h"
#include "OtherUtil.h"
#include "PlayerManager.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	NotificationCenter::getInstance()->removeAllObservers(this);
}

Scene* GameScene::createScene(PlayerManager *playerManage)
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
	//添加UI层
	m_ui = GameUI::create();
	this->addChild(m_ui, 10);
	//建立收信方，监听玩家血量和蓝量
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::UpdateUI),
		"updateUI",
		NULL
	);

	//随机选定地图
	m_mapOrder = rand() % m_mapNum;

	//debug
	//int a[20][30];
	//for (int i = ;i < 20;i++)
	//{
	//	for (int j = 0;j < 30;j++)
	//	{
	//		if (OtherUtil::isContain(m_canStandTileID.at(m_mapOrder), (int)m_tileMap->getLayer(MAP_LAYER)->getTileGIDAt(Vec2(j, i))))
	//		{
	//			a[i][j] = 1;
	//		}
	//		else
	//		{
	//			a[i][j] = 0;
	//		}
	//	}
	//}

	//根据地图名读取文件
	m_tileMap = TMXTiledMap::create(m_mapNames.at(m_mapOrder));
	this->addChild(m_tileMap);

	//注册键盘监听函数
	auto listener = EventListenerKeyboard::create();
	//定义按键按下
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//加入按下按键为已定义按键
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				m_playerManager->DoActionByKeyCode(i, keyCode);
				//左跑
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 1)
				{
					m_playerManager->changePlayerRunDirByID(i, -1);
				}
				//右跑
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager->changePlayerRunDirByID(i, 1);
				}
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) != 4 && PlayerManager::GetKeyIDByKeyCode(keyCode) != 10)
				{
					m_playerManager->ReLoadActionByID(i);
				}
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
					m_playerManager->changePlayerRunDirByID(i, 1);
				}
				//之前右跑
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager->changePlayerRunDirByID(i, -1);
				}
				m_playerManager->ReLoadActionByID(i);
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
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//计算ID
	int mapx = x / m_tileMap->getTileSize().width;
	mapx = mapx >= 0 ? mapx : 0;
	int mapy = (visibleSize.height - y) / m_tileMap->getTileSize().height;
	mapy = mapy >= 0 ? mapy : 0;
	if (OtherUtil::isContain(m_canStandTileID.at(m_mapOrder),int(m_tileMap->getLayer(MAP_LAYER)->getTileGIDAt(Vec2(mapx, mapy+1)))-1))
	{
		return true;
	}
	return false;
}

void GameScene::loadMapInfo(const char* file)
{
	//执行lua脚本读取场景
	lua_State* pL = LuaUtil::openLuaFile(file);

	//获取地图
	//获取地图数量
	lua_getglobal(pL, "getMapsLen");
	lua_call(pL, 0, 1);
	m_mapNum = lua_tonumber(pL, -1);

	//获取地图名
	for (int i = 0;i < m_mapNum;i++)
	{
		lua_getglobal(pL, "maps");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* m = lua_tostring(pL, -1);
		m_mapNames.push_back(Value(m).asString());
	}

	//获取玩家能站立地图ID
	for (int i = 0;i < m_mapNum;i++)
	{
		m_canStandTileID.push_back(std::vector<int>());

		lua_getglobal(pL, "getCanStandTileNumByID");
		lua_pushinteger(pL, i);
		lua_call(pL, 1, 1);
		int tileNum = lua_tointeger(pL, -1);

		for (int j = 0;j < tileNum;j++)
		{
			lua_getglobal(pL, "canStandTileID");
			lua_pushinteger(pL, i + 1);
			lua_gettable(pL, -2);

			lua_pushinteger(pL, j + 1);
			lua_gettable(pL, -2);
			m_canStandTileID.at(i).push_back(lua_tointeger(pL, -1));
		}
	}

	LuaUtil::closeLuaFile(pL);
}

std::vector<std::string> GameScene::m_mapNames;

int GameScene::m_mapNum;

int GameScene::m_mapOrder;

std::vector<std::vector<int>> GameScene::m_canStandTileID;

TMXTiledMap * GameScene::m_tileMap;

void GameScene::update(float dt)
{
	//检测是否有攻击和受到攻击
	if (m_playerManager->IsPlayersCollide())
	{
		//任何1人受伤则不会再受伤
		if (m_playerManager->GetBattleStateByID(0) == HURT || m_playerManager->GetBattleStateByID(1) == HURT)
		{
			return;
		}

		//玩家1是否对玩家2造成伤害
		if (m_playerManager->isPlayer1Left() && m_playerManager->GetTowardsByID(0) == 1
			|| !m_playerManager->isPlayer1Left() && m_playerManager->GetTowardsByID(0) == -1)
		{
			if (m_playerManager->GetBattleStateByID(0) == ATTACK)
			{
				m_playerManager->ForceToHurtByID(1, Player::GetKickHurt());
			}
			else if (m_playerManager->GetBattleStateByID(0) == UATTACK)
			{
				m_playerManager->ForceToHurtByID(1, Player::GetUltimateSkillHurt());
			}
		}
		//玩家2对玩家1造成伤害
		if (m_playerManager->isPlayer1Left() && m_playerManager->GetTowardsByID(1) == -1
			|| !m_playerManager->isPlayer1Left() && m_playerManager->GetTowardsByID(1) == 1)
		{
			if (m_playerManager->GetBattleStateByID(1) == ATTACK)
			{
				m_playerManager->ForceToHurtByID(0, Player::GetKickHurt());
			}
			else if (m_playerManager->GetBattleStateByID(1) == UATTACK)
			{
				m_playerManager->ForceToHurtByID(0, Player::GetUltimateSkillHurt());
			}
		}
	}
}

void GameScene::UpdateUI(Ref* pSender)
{
	UIMessage* uiMessage = (UIMessage*)pSender;
	m_ui->SetHealthByID(uiMessage->id, uiMessage->health);
	m_ui->SetPowerByID(uiMessage->id, uiMessage->power);
}
