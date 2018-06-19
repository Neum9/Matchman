/*
	游戏管理类
	管理游戏运行逻辑
*/

#pragma once
#include "GameScene.h"
#include "PlayerManager.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	//获取游戏场景
	Scene* getGameScene();
		
private:
	Scene* m_GameScene;
	PlayerManager* m_playerManager;
};