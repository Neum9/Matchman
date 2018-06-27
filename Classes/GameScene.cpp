#include "GameScene.h"
#include "LuaUtil.h"
#include "OtherUtil.h"
#include "PlayerManager.h"
#include "LoadScene.h"

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
	layer->setName("gameScene");
	layer->m_playerManager = playerManage;
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//添加胜利背景
	m_winBG = Sprite::create("background/WinBG.png");
	this->addChild(m_winBG, 5);
	m_winBG->runAction(FadeOut::create(0.0F));
	m_winBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//添加UI层
	m_ui = GameUI::create();
	this->addChild(m_ui, 3);
	//建立收信方，监听玩家血量和蓝量
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::UpdateUI),
		"updateUI",
		NULL
	);
	//监听玩家得分
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::UpdateScore),
		"updateScore",
		NULL
	);
	//添加收信方，监听玩家胜利
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::ResetScene),
		"resetSceneInGameScene",
		NULL
	);
	//添加收信，监听暂停事件
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::Pause),
		"pause",
		NULL
	);
	//添加收信，监听大招
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameScene::ShowULBG),
		"showULBG",
		NULL
	);


	//随机添加背景图
	int backOrder = rand() % m_backgrounds.size();
	std::string path = m_backgrounds.at(backOrder);
	auto bg = Sprite::create(path);
	this->addChild(bg, 1);
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	//随机选定地图
	m_mapOrder = rand() % m_mapNum;

	//根据地图名读取文件
	m_tileMap = TMXTiledMap::create(m_mapNames.at(m_mapOrder));
	this->addChild(m_tileMap,2);

	//添加大招背景
	m_ultimateSkillBG = Sprite::create("background/UlimateSkillBG.png");
	m_ultimateSkillBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(m_ultimateSkillBG, 5);
	m_ultimateSkillBG->setVisible(false);

	//注册键盘监听函数
	m_listener = EventListenerKeyboard::create();
	//定义按键按下
	m_listener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event * event) {
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
					log("left press!");
				}
				//右跑
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager->changePlayerRunDirByID(i, 1);
					log("right press!");
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
	m_listener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event * event) {
		//检测松开的按键为哪个player的
		for (int i = 0;i < PlayerManager::getAllKey().size();i++)
		{
			if (OtherUtil::isContain(PlayerManager::getAllKey().at(i), keyCode))
			{
				//之前左跑
				if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 1)
				{
					m_playerManager->changePlayerRunDirByID(i, 1);
					log("left release!");
				}
				//之前右跑
				else if (PlayerManager::GetKeyIDByKeyCode(keyCode) == 2)
				{
					m_playerManager->changePlayerRunDirByID(i, -1);
					log("right release!");
				}
				m_playerManager->ReLoadActionByID(i);
				return;
			}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	//添加定时器
	this->scheduleUpdate();

	return true;
}

bool GameScene::isTileCanbeStand(float x, float y)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//计算ID
	int mapx = (x - m_tileMap->getTileSize().width/2) / m_tileMap->getTileSize().width;
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

	//获取背景图
	lua_getglobal(pL, "getBackGroundNum");
	lua_call(pL, 0, -1);
	int backNum = lua_tointeger(pL, -1);

	for (int i = 0;i < backNum;i++)
	{
		lua_getglobal(pL, "background");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* c = lua_tostring(pL, -1);
		m_backgrounds.push_back(Value(c).asString());
	}

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

void GameScene::ShowWinBGInTime(float t)
{
	m_winBG->runAction(FadeIn::create(t));
	this->runAction(Sequence::create(DelayTime::create(t+1.5F), CallFunc::create([]() {
		Director::getInstance()->replaceScene(LoadScene::createScene());
	}), nullptr));
}

std::vector<std::string> GameScene::m_mapNames;

int GameScene::m_mapNum;

int GameScene::m_mapOrder;

std::vector<std::vector<int>> GameScene::m_canStandTileID;

std::vector<std::string> GameScene::m_backgrounds;

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
				log("1 hurt 2");
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

void GameScene::Pause(Ref* pSender)
{
	if (!Director::getInstance()->isPaused())
	{
		Director::getInstance()->pause();
		_eventDispatcher->pauseEventListenersForTarget(this);
	}
	else
	{
		Director::getInstance()->resume();
		_eventDispatcher->resumeEventListenersForTarget(this);
	}
}

void GameScene::UpdateUI(Ref* pSender)
{
	UIMessage* uiMessage = (UIMessage*)pSender;
	m_ui->SetHealthByID(uiMessage->id, uiMessage->health);
	m_ui->SetPowerByID(uiMessage->id, uiMessage->power);
}

void GameScene::UpdateScore(Ref* pSender)
{
	UIScore* uiScore = (UIScore*)pSender;
	m_ui->SetScoreByID(uiScore->id, uiScore->score);
}

void GameScene::ResetScene(Ref* pSender)
{
	//更新得分
	int failID = (int)pSender;
	int winID = 1 - failID;
	//首先解除键盘控制
	_eventDispatcher->removeAllEventListeners();
	//先停止玩家行动
	m_playerManager->changePlayerRunDirByID(winID, -(m_playerManager->getPlayerAtIDRunDir(winID)-1));
	m_playerManager->ReLoadActionByID(winID);

	auto sendMsg = CallFunc::create([=]() {
		NotificationCenter::getInstance()->postNotification("resetSceneInGameManager", (Ref*)winID);
	});

	//传递赢的玩家
	this->runAction(Sequence::create(DelayTime::create(2.0F), sendMsg, nullptr));
}

void GameScene::ShowULBG(Ref* pSender)
{
	m_ultimateSkillBG->setVisible(true);
	m_ultimateSkillBG->runAction(Sequence::create(DelayTime::create(2.0F), CallFunc::create([&]() {
		m_ultimateSkillBG->setVisible(false);
	}), nullptr));
}
