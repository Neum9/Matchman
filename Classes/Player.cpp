#include "Player.h"
#include "LuaUtil.h"

Player::Player()
{

}

Player::~Player()
{

}


Player* Player::create(int id, int type)
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

bool Player::init(int id, int type)
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
	lua_pushinteger(pL, type);
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
