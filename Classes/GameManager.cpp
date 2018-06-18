#include "GameManager.h"
#include "PlayerType.h"

GameManager::GameManager()
{
	//从脚本读取玩家类型
	PlayerType::LoadTypeFromLua();
	//创建游戏场景
	m_GameScene = GameScene::createScene();
	m_GameScene->setTag(9);
	//创建玩家管理器
	m_playerManager = new PlayerManager(m_GameScene);
}

GameManager::~GameManager()
{

}

Scene* GameManager::getGameScene()
{
	return m_GameScene;
}
