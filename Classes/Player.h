/*
	玩家类
	管理玩家操纵角色逻辑
*/

#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PlayerType.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;



//玩家类
class Player : public Sprite
{
public:
	Player();
	~Player();
	static Player* create(int id, int type);
private:
	bool init(int id, int type);

	int m_playerID;
	int m_playerType;
	Armature *m_armature;
};