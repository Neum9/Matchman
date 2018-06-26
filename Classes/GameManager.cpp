#include "GameManager.h"
#include "PlayerType.h"
#include "Player.h"
#include "OtherUtil.h"
#include "MusicUtil.h"

GameManager::GameManager()
{
	//从脚本读取玩家类型
	PlayerType::LoadTypeFromLua("luaData/testData.lua");
	//从脚本读取玩家动作类型
	Player::LoadPlayerActionFromLua("luaData/testData.lua");
	//从脚本读取玩家的按键和动作的对应关系
	PlayerManager::LoadActionKeyAndIDFromLua("luaData/testData.lua");
	//从脚本读取地图信息
	GameScene::loadMapInfo("luaData/testData.lua");

	//产生种子用于随机
	srand(time(NULL));

	//创建玩家管理器
	m_playerManager = new PlayerManager();
	//创建游戏场景
	m_GameScene = GameScene::createScene(m_playerManager);
	m_playerManager->AddPlayerToScene(m_GameScene);
	//玩家胜利次数初始化
	m_finalNeedScore = 3;
	m_playerScore = new std::vector<int>();
	m_playerScore->push_back(0);
	m_playerScore->push_back(0);
	//建立收信方
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameManager::resetScene),
		"resetSceneInGameManager",
		NULL
	);
	//预加载音乐 播放背景
	MusicUtil::preLoadAllMusic();
	//播放背景音乐
	MusicUtil::playBackGroundMusic();
}

GameManager::~GameManager()
{
	//移除收信
	NotificationCenter::getInstance()->removeAllObservers(this);
}

Scene* GameManager::getGameScene()
{
	return m_GameScene;
}

void GameManager::resetScene(Ref* pSender)
{
	m_GameScene->cleanup();
	m_playerManager->ReloadPlayer();
	m_GameScene = GameScene::createScene(m_playerManager);
	m_playerManager->AddPlayerToScene(m_GameScene);

	//获取胜利ID，得分+1
	int winID = (int)pSender;
	m_playerScore->at(winID) += 1;
	//更换场景
	Director::getInstance()->replaceScene(m_GameScene);
	//发送更改score
	UIScore* uiScore = new UIScore();
	uiScore->id = winID;
	uiScore->score = m_playerScore->at(winID);
	NotificationCenter::getInstance()->postNotification("updateScore", uiScore);
	uiScore->id = 1-winID;
	uiScore->score = m_playerScore->at(1-winID);
	NotificationCenter::getInstance()->postNotification("updateScore", uiScore);
}
