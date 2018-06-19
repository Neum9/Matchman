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