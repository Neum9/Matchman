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

//跑动方向
enum RunDir
{
	RUNLEFT,
	STAND,
	RUNRIGHT
};

//战斗状态
enum BattleState
{
	NONE,
	ATTACK,
	UATTACK,
	HURT
};


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

	RunDir getRunDir();
	//根据change改变dir
	void changeRunDir(int change);
	bool isActionEnd();

	//重新判断动作 run stand
	void ReLoadAction();

	//得到面向的方向-1 左 1 右
	int GetTowards();
	//得到攻击状态
	BattleState GetBattleState();
	//强制打断动作受伤
	void ForceToHurt(int damage);
	//得到动画播放位置
    float GetAnimationPositionX();
	
	static int GetPunchHurt() { return m_punchHurt; }
	static int GetKickHurt() { return m_kickHurt; }
	static int GetUltimateSkillHurt() { return m_ultimateSkillHurt; }

	//得到动画
	Sprite* getAnimature();
private:
	//所有动作类型
	static std::vector<std::string> m_playerActionType;
	//玩家移动单位
	static float m_playerMoveUnit;
	//玩家跳跃单位
	static float m_playerJumpUnit;
	//玩家总血量
	static int m_maxHealth;
	//玩家释放大招需要能量
	static int m_maxUltimateSlillNeed;
	//玩家技能伤害
	static int m_punchHurt;
	static int m_kickHurt;
	static int m_ultimateSkillHurt;
	//使用小技能增加的能量
	static int m_addPower;

	bool init(int id, std::string type);

	//玩家信息
	int m_playerID;
	std::string m_playerType;
	//玩家血量 能量
	int m_nowHealth;
	int m_nowPower;
	//战斗状态
	BattleState m_battleState;
	//动画
	Armature *m_armature;
	//目前执行的动作
	std::string m_nowAction;
	//是否可以从nowAction转换到newAction
	bool CanTurnTo(std::string newAction);
	//某些不可循环的Action需要等待end结束动作
	bool m_isActionEnd;
	//跑动方向
	RunDir m_runDir;
	//帧监听
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	//所有end事件
	static std::vector<std::string> m_endEvent;
	//重写update函数
	void update(float dt) override;
	//是否处于跳跃状态
	bool m_isJump;
	//是否处于下落状态
	bool m_isFall;
	//查看是否超出范围
	void checkRange();
	//发信更新UI信息
	void SendToUpateUI();
	//粒子效果
	ParticleSystemQuad* m_hurtEffect;
};