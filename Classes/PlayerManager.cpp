#include "PlayerManager.h"
#include "LuaUtil.h"
#include "OtherUtil.h"

//静态变量定义
std::map<EventKeyboard::KeyCode, int> PlayerManager::m_actionKeyAndID;

std::vector<std::vector<EventKeyboard::KeyCode>> PlayerManager::m_allKey;

void PlayerManager::TurnActionByKeyCode(Ref* pSender)
{

}

PlayerManager::PlayerManager()
{
	//写死测试
	m_players.push_back(Player::create(0, PlayerType::GetPlayerTypeByOrder(0)));
	m_players.push_back(Player::create(1, PlayerType::GetPlayerTypeByOrder(1)));
}

PlayerManager::~PlayerManager()
{
	m_players.clear();
}

void PlayerManager::ReloadPlayer()
{
	m_players.clear();
	//写死测试
	m_players.push_back(Player::create(0, PlayerType::GetPlayerTypeByOrder(0)));
	m_players.push_back(Player::create(1, PlayerType::GetPlayerTypeByOrder(1)));
}

Player *PlayerManager::getPlayerByID(int i)
{
	return m_players.at(i);
}

void PlayerManager::AddPlayerToScene(Scene* sceneToAdd)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//将玩家添加进场景
	for (int i = 0;i < m_players.size();i++)
	{
		sceneToAdd->addChild(m_players.at(i));
	}
}

void PlayerManager::LoadActionKeyAndIDFromLua(const char* file)
{
	

	lua_State* pL = LuaUtil::openLuaFile(file);

	//读取玩家数量
	lua_getglobal(pL, "playerNum");
	int playerNum = lua_tonumber(pL, -1);

	//根据玩家数量添加key vector 和 keypress vector
	for (int i = 0;i < playerNum;i++)
	{
		m_allKey.push_back(std::vector<EventKeyboard::KeyCode>());
	}

	//获取按键数量（单个玩家）
	lua_getglobal(pL, "getKeyTypeNum");
	lua_call(pL, 0, 1);
	int keyNum = lua_tonumber(pL, -1);

	//根据玩家数量读取每个玩家的键盘控制
	for (int i = 0;i < playerNum;i++)
	{
		for (int j = 0;j < keyNum;j++)
		{
			lua_getglobal(pL, "allPlayerKey");
			lua_pushinteger(pL, i + 1);
			lua_gettable(pL, -2);
			lua_pushinteger(pL, j + 1);
			lua_gettable(pL, -2);
			const char* c = lua_tostring(pL, -1);
			lua_getglobal(pL, "getActionIDByKey");
			lua_pushstring(pL, c);
			lua_call(pL, 1, 1);
			EventKeyboard::KeyCode kc = getKeyCodeByKeyString(c);
			//如果键盘符属于没有输入过的则加入数组中
			if (!OtherUtil::isContain(m_allKey.at(i), kc))
			{
				m_allKey.at(i).push_back(kc);
			}
			int id = lua_tointeger(pL, -1);
			m_actionKeyAndID.insert(std::make_pair(kc, id));
		}
	}
	LuaUtil::closeLuaFile(pL);
}

std::vector<std::vector<EventKeyboard::KeyCode>> PlayerManager::getAllKey()
{
	return m_allKey;
}

int PlayerManager::GetKeyIDByKeyCode(EventKeyboard::KeyCode keyCode)
{
	return m_actionKeyAndID.at(keyCode);
}

void PlayerManager::DoActionByKeyCode(int playerID, EventKeyboard::KeyCode keyCode)
{
	//执行动画
	std::string action = Player::getPlayerActionTypeByID(m_actionKeyAndID.at(keyCode));
	m_players.at(playerID)->TryTurnTo(action);
}

bool PlayerManager::isPlayerAtIDEnd(int id)
{
	return m_players.at(id)->isActionEnd();
}

RunDir PlayerManager::getPlayerAtIDRunDir(int id)
{
	return m_players.at(id)->getRunDir();
}

void PlayerManager::changePlayerRunDirByID(int id,int change)
{
	m_players.at(id)->changeRunDir(change);
}

void PlayerManager::ReLoadActionByID(int id)
{
	m_players.at(id)->ReLoadAction();
}

bool PlayerManager::IsPlayersCollide()
{
	bool result = PixelCollision::getInstance()->collidesWithSprite(m_players.at(0)->getAnimature(), m_players.at(1)->getAnimature());
	return result;
}

int PlayerManager::GetTowardsByID(int id)
{
	return m_players.at(id)->GetTowards();
}

BattleState PlayerManager::GetBattleStateByID(int id)
{
	return m_players.at(id)->GetBattleState();
}

bool PlayerManager::isPlayer1Left()
{
	//test
	int a = m_players.at(0)->GetAnimationPositionX();
	int b = m_players.at(1)->GetAnimationPositionX();
	if (m_players.at(0)->GetAnimationPositionX() < m_players.at(1)->GetAnimationPositionX())
	{
		return true;
	}
	return false;
}

void PlayerManager::ForceToHurtByID(int id, int damage)
{
	m_players.at(id)->ForceToHurt(damage);
}

EventKeyboard::KeyCode PlayerManager::getKeyCodeByKeyString(std::string key)
{
	if (key == "a")
	{
		return EventKeyboard::KeyCode::KEY_A;
	} 
	else if (key == "d")
	{
		return EventKeyboard::KeyCode::KEY_D;
	}
	else if (key == "w")
	{
		return EventKeyboard::KeyCode::KEY_W;
	}
	else if (key == "s")
	{
		return EventKeyboard::KeyCode::KEY_S;
	}
	else if (key == "j")
	{
		return EventKeyboard::KeyCode::KEY_J;
	}
	else if (key == "k")
	{
		return EventKeyboard::KeyCode::KEY_K;
	}
	else if (key == "l")
	{
		return EventKeyboard::KeyCode::KEY_L;
	}
	else if (key == "i")
	{
		return EventKeyboard::KeyCode::KEY_I;
	}
	else if (key == "o")
	{
		return EventKeyboard::KeyCode::KEY_O;
	}
	else if (key == "left")
	{
		return EventKeyboard::KeyCode::KEY_LEFT_ARROW;
	}
	else if (key == "right")
	{
		return EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	}
	else if (key == "up")
	{
		return EventKeyboard::KeyCode::KEY_UP_ARROW;
	}
	else if (key == "down")
	{
		return EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	}
	else if (key == "1")
	{
		return EventKeyboard::KeyCode::KEY_1;
	}
	else if (key == "2")
	{
		return EventKeyboard::KeyCode::KEY_2;
	}
	else if (key == "3")
	{
		return EventKeyboard::KeyCode::KEY_3;
	}
	else if (key == "5")
	{
		return EventKeyboard::KeyCode::KEY_5;
	}
	else if (key == "6")
	{
		return EventKeyboard::KeyCode::KEY_6;
	}
	else
	{
		log("Not such key!");
		return EventKeyboard::KeyCode::KEY_0;
	}
}
