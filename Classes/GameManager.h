/*
	游戏管理类
	管理游戏运行逻辑
*/

#pragma once
#include "GameScene.h"
#include "PlayerManager.h"
#include <ctime>

class GameManager : public Ref
{
public:
	GameManager();
	~GameManager();

	//获取游戏场景
	Scene* getGameScene();
		
private:

	Scene* m_GameScene;
	PlayerManager* m_playerManager;

	//有1方被打倒重启场景
	void resetScene(Ref* pSender);

	//总胜利需要次数
	int m_finalNeedScore;

	//2位玩家胜利次数
	std::vector<int> *m_playerScore;
};