/*
	游戏场景类
	游戏场景逻辑管理
*/

#pragma once
#include "cocos2d.h"
#include "PlayerManager.h"

USING_NS_CC;


class GameScene : public Layer
{
public:
	GameScene();
	~GameScene();
	static Scene* createScene(PlayerManager playerManager);
    bool init() override;
	CREATE_FUNC(GameScene);
	static bool isTileCanbeStand(float x, float y);
private:
	//地图信息：地图名，地图数量，第几个地图
	static std::vector<std::string> m_mapNames;
	static int m_mapNum;
	static int m_mapOrder;

	PlayerManager m_playerManager;

	void update(float dt) override;
};