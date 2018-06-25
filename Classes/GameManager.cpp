#include "GameManager.h"
#include "PlayerType.h"
#include "Player.h"

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

	//建立收信方
	NotificationCenter::getInstance()->addObserver(
		this,
		callfuncO_selector(GameManager::resetScene),
		"resetScene",
		NULL
	);
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
	//更换场景
	//auto delay = DelayTime::create(2.0F);
	//auto replaceScene = CallFunc::create([&]() {
	//	Director::getInstance()->replaceScene(m_GameScene);
	//});
	//this->runAction(Sequence::create(delay, replaceScene, nullptr));
	Director::getInstance()->replaceScene(m_GameScene);
}
