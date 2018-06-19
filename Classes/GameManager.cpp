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
	//创建玩家管理器
	m_playerManager = new PlayerManager();
	//创建游戏场景
	m_GameScene = GameScene::createScene(*m_playerManager);
	m_playerManager->AddPlayerToScene(m_GameScene);
}

GameManager::~GameManager()
{

}

Scene* GameManager::getGameScene()
{
	return m_GameScene;
}
