#include "Player.h"
#include "LuaUtil.h"
#include "OtherUtil.h"

std::vector<std::string> Player::m_playerActionType;

void Player::LoadPlayerActionFromLua(const char* file)
{
	lua_State* pL = LuaUtil::openLuaFile(file);
	//获取动作列表数量
	lua_getglobal(pL, "getPlayerActionTypeNum");
	lua_call(pL, 0, 1);
	int actionTypeNum = lua_tonumber(pL, -1);
	//读取动作列表
	for (int i = 0;i < actionTypeNum;i++)
	{
		lua_getglobal(pL, "playerAction");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* c = lua_tostring(pL, -1);
		m_playerActionType.push_back(Value(c).asString());
	}
	LuaUtil::closeLuaFile(pL);
}

bool Player::TryTurnTo(std::string newAction)
{
	//新Action不在已存类型中
	if (!OtherUtil::isContain(m_playerActionType, newAction))
	{
		log("%s not exist!", newAction);
		return false;
	}
	//可以转换动作
	log("Player %d:nowAction: %s", m_playerID,newAction.c_str());
	m_nowAction = newAction;

	//对run特殊处理

	//向左跑
	if (m_nowAction == m_playerActionType.at(0))
	{
		m_armature->setScaleX(-1);
		m_armature->getAnimation()->play("run");
	}
	//向右跑
	else if (m_nowAction == m_playerActionType.at(1))
	{
		m_armature->setScaleX(1);
		m_armature->getAnimation()->play("run");
	}
	else
	{
		m_armature->getAnimation()->play(m_nowAction);
	}

	return true;
}

std::string Player::getPlayerTypeByID(int actionID)
{
	return m_playerActionType.at(actionID);
}

Player::Player()
{
	m_nowAction = m_playerActionType.at(0);
}

Player::~Player()
{

}


Player* Player::create(int id, std::string type)
{
	Player *pRet = new(std::nothrow) Player();
		if (pRet && pRet->init(id,type)) 
		{ 
			pRet->autorelease(); 
			return pRet; 
		} 
		else 
		{ 
			delete pRet; 
			pRet = nullptr; 
			return nullptr; 
		} 
}

bool Player::init(int id, std::string type)
{
	if (!Sprite::init())
	{
		return false;
	}
	//成员赋值
	m_playerID = id;
	m_playerType = type;

	//根据playerType执行lua脚本读取动画文件
	lua_State* pL = LuaUtil::openLuaFile("luaData/testData.lua");
	lua_getglobal(pL, "getAnimationFile");
	lua_pushstring(pL, type.c_str());
	lua_call(pL, 1, 1);

	const char* c = lua_tostring(pL, -1);

	//添加动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo(c);
	m_armature = Armature::create("MatchmanTest");
	if (m_armature == NULL)
	{
		log("player %d animation load error!", m_playerID);
		return false;
	}
	m_armature->setPosition(Vec2::ZERO);

	m_armature->getAnimation()->play("run");
	this->setContentSize(m_armature->getContentSize());
	this->addChild(m_armature);


	return true;
}
