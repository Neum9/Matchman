/*
	玩家管理类
	管理玩家角色交互逻辑
*/

#pragma once
#include "Player.h"
#include "cocos2d.h"
#include <map>
#include <vector>
#include "PixelCollision.h"

USING_NS_CC;

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();
	Player *getPlayerByID(int i);

	//将Player加到场景中
	void AddPlayerToScene(Scene* sceneToAdd);

	//读取按键和动作的对应关系
	static void LoadActionKeyAndIDFromLua(const char* file);
	static std::vector<std::vector<EventKeyboard::KeyCode>> getAllKey();
	static int GetKeyIDByKeyCode(EventKeyboard::KeyCode keyCode);
	//根据KeyCode执行玩家Action
	void DoActionByKeyCode(int playerID, EventKeyboard::KeyCode keyCode);
	//设置player为stand
	void SetPlayerStand(int playerID);
	//查看某个玩家Action是否结束
	bool isPlayerAtIDEnd(int id);
	//查看某个玩家跑动方向
	RunDir getPlayerAtIDRunDir(int id);
	//改变某个玩家跑动方向
	void changePlayerRunDirByID(int id,int change);
	//重新判断玩家方向
	void ReLoadActionByID(int id);
    bool IsPlayersCollide();
private:
	//玩家
	std::vector<Player*> m_players;
	//记录按键和动作ID
	static std::map<EventKeyboard::KeyCode, int> m_actionKeyAndID;
	//根据按键字符串转成键盘符
	static EventKeyboard::KeyCode getKeyCodeByKeyString(std::string key);
	//记录已经有的按键
	static std::vector<std::vector<EventKeyboard::KeyCode>> m_allKey;
	void TurnActionByKeyCode(Ref* pSender);
};