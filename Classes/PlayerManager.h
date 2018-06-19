/*
	玩家管理类
	管理玩家角色交互逻辑
*/

#pragma once
#include "Player.h"
#include "cocos2d.h"
#include <map>
#include <vector>

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
	//根据KeyCode执行玩家Action
	void DoActionByKeyCode(int playerID, EventKeyboard::KeyCode keyCode);
	//设置player为stand
	void SetPlayerStand(int playerID);
	//设置按键按下和松开状态
	static void setKeyPressed(int playerID,bool isPressed);
	static bool getKeyPressed(int playerID);
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
	//按键是否被按下,计数，当按键配按下计数+1，松开-1，为0代表没有按键被按下
	static std::vector<int> m_isKeyPressed;
};