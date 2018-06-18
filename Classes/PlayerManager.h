/*
	玩家管理类
	管理玩家角色交互逻辑
*/

#pragma once
#include "Player.h"

class PlayerManager
{
public:
	PlayerManager(Scene* sceneToAdd);
	~PlayerManager();
	Player *getPlayerByID(int i);
private:
	//玩家
	std::vector<Player*> m_players;
};