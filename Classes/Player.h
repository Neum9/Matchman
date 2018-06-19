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
	static Player* create(int id, std::string type);
	static void LoadPlayerActionFromLua(const char* file);

	//动作的转换
	bool TryTurnTo(std::string newAction);
	//通过ActionID得到Action类型
	static std::string getPlayerActionTypeByID(int actionID);
	static int getActionIDByActionType(std::string actionType);
private:

	static std::vector<std::string> m_playerActionType;

	bool init(int id, std::string type);

	//玩家信息
	int m_playerID;
	std::string m_playerType;
	//动画
	Armature *m_armature;
	//目前执行的动作
	std::string m_nowAction;
	//是否可以从nowAction转换到newAction
	bool CanTurnTo(std::string newAction);
	//某些不可循环的Action需要等待end结束动作
	bool m_isActionEnd;
	//test 消除由于帧长不同引起的差异
	bool m_isActionEndoffset;
	bool m_isFrameOffset;
	//帧监听
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	//所有end事件
	static std::vector<std::string> m_endEvent;
};