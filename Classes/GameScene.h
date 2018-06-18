/*
	游戏场景类
	游戏场景逻辑管理
*/

#pragma once
#include "cocos2d.h"

USING_NS_CC;


class GameScene : public Layer
{
public:
	GameScene();
	~GameScene();

	static Scene* createScene();
    bool init() override;
	CREATE_FUNC(GameScene);
private:
	//地图信息：地图名，地图数量，第几个地图
	std::vector<std::string> m_mapNames;
	int m_mapNum;
	int m_mapOrder = 0;
};