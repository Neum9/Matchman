#include "Player.h"
#include "LuaUtil.h"
#include "OtherUtil.h"
#include "PlayerManager.h"
#include "GameScene.h"
#include <algorithm>
#include <iterator>

//向右
void setPositive(RunDir &r)
{
	switch (r)
	{
	case RUNLEFT:
		r = RunDir::STAND;
		break;
	case STAND:
		r = RunDir::RUNRIGHT;
		break;
	case RUNRIGHT:
		r = RunDir::RUNRIGHT;
		break;
	default:
		break;
	}
}

//向左
void setNegetive(RunDir &r)
{
	switch (r)
	{
	case RUNLEFT:
		r = RunDir::RUNLEFT;
		break;
	case STAND:
		r = RunDir::RUNLEFT;
		break;
	case RUNRIGHT:
		r = RunDir::STAND;
		break;
	default:
		break;
	}
}

std::vector<std::string> Player::m_playerActionType;

float Player::m_playerMoveUnit;

float Player::m_playerJumpUnit;

int Player::m_maxHealth;

int Player::m_maxUltimateSlillNeed;

int Player::m_punchHurt;

int Player::m_kickHurt;

int Player::m_ultimateSkillHurt;

int Player::m_addPower;

std::vector<std::string> Player::m_endEvent;

void Player::update(float dt)
{
	//控制位置
	int actionID = getActionIDByActionType(m_nowAction);
	switch (actionID)
	{
	case 1:
		//向左
		m_armature->setPositionX(m_armature->getPositionX() - m_playerMoveUnit);
		break;
	case 2:
		//向右
		m_armature->setPositionX(m_armature->getPositionX() + m_playerMoveUnit);
		break;
	case 3:
		//跳跃
		m_armature->setPositionY(m_armature->getPositionY() + m_playerJumpUnit * 5 / 4);
		switch (m_runDir)
		{
		case RUNLEFT:
			m_armature->setPositionX(m_armature->getPositionX() - m_playerMoveUnit);
			break;
		case STAND:
			break;
		case RUNRIGHT:
			m_armature->setPositionX(m_armature->getPositionX() + m_playerMoveUnit);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	//检测是否超出范围
	checkRange();

	//检测是否需要下落
	if (!GameScene::isTileCanbeStand(m_armature->getPositionX(), m_armature->getPositionY()))
	{
		m_canstand = false;
		m_armature->setPositionY(m_armature->getPositionY() - m_playerJumpUnit * 3 / 4);
		//设置状态
		if (actionID != 3)
		{
			m_isFall = true;
		}
	}
	else
	{
		if (m_canstand == false)
		{
			//判断跑动方向
			Player::ReLoadAction();
		}
		m_canstand = true;
		if (actionID != 4 && actionID != 10 && actionID != 11)
		{
			if (m_isFall = true)
			{
				//判断跑动方向
				Player::ReLoadAction();
			}
			m_isFall = false;
		}
	}



	//drawNode->clear();
	//Vec2 o = m_armature->getBoundingBox().origin;
	//Vec2 s = m_armature->getBoundingBox().size;
	//auto pos = this->getPosition();
	//auto box = this->getBoundingBox();
	//drawNode->drawRect(m_armature->getBoundingBox().origin, m_armature->getBoundingBox().origin + m_armature->getBoundingBox().size, Color4F(0.0F, 1.0F, 0.0F, 1.0F));
	//drawNode->drawRect(this->getBoundingBox().origin, this->getBoundingBox().origin + this->getBoundingBox().size, Color4F(1.0F, 0.0F, 0.0F, 1.0F));


	//pos = this->getPosition();
	//box = this->getBoundingBox();
}

void Player::checkRange()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//检测是否超出范围
	if (m_armature->getPositionX() > visibleSize.width-10)
	{
		m_armature->setPositionX(visibleSize.width-10);
	}
	if (m_armature->getPositionX() < 0)
	{
		m_armature->setPositionX(0);
	}
	if (m_armature->getPositionY() > visibleSize.height)
	{
		m_armature->setPositionY(visibleSize.height);
	}
	if (m_armature->getPositionY() < 0)
	{
		m_armature->setPositionY(0);
	}
}

void Player::SendToUpateUI()
{
	UIMessage* uiMessage = new UIMessage();
	uiMessage->id = m_playerID;
	uiMessage->health = m_nowHealth;
	uiMessage->power = m_nowPower;
	NotificationCenter::getInstance()->postNotification("updateUI", uiMessage);
}

void Player::LoadPlayerActionFromLua(const char* file)
{
	lua_State* pL = LuaUtil::openLuaFile(file);
	//读取动作
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
	//读取动作结束
	//读取动作结束事件数量
	lua_getglobal(pL, "getEndEventNum");
	lua_call(pL, 0, 1);
	int endEventNum = lua_tonumber(pL, -1);
	//读取动作结束事件
	for (int i = 0;i < endEventNum;i++)
	{
		lua_getglobal(pL, "endEvent");
		lua_pushinteger(pL, i + 1);
		lua_gettable(pL, -2);
		const char* c = lua_tostring(pL, -1);
		m_endEvent.push_back(Value(c).asString());
	}

	//读取玩家移动单位
	lua_getglobal(pL, "playerMoveUnit");
	m_playerMoveUnit = lua_tonumber(pL, -1);
	//读取玩家跳跃单位
	lua_getglobal(pL, "playerJumpUnit");
	m_playerJumpUnit = lua_tonumber(pL, -1);

	//读取玩家健康，技能信息
	lua_getglobal(pL, "playerHealth");
	m_maxHealth = lua_tointeger(pL, -1);

	lua_getglobal(pL, "playerultimateSkillNeed");
	m_maxUltimateSlillNeed = lua_tointeger(pL, -1);

	lua_getglobal(pL, "punchHurt");
	m_punchHurt = lua_tointeger(pL, -1);

	lua_getglobal(pL, "kickHurt");
	m_kickHurt = lua_tointeger(pL, -1);

	lua_getglobal(pL, "ultimateSkillHurt");
	m_ultimateSkillHurt = lua_tointeger(pL, -1);

	lua_getglobal(pL, "addPower");
	m_addPower = lua_tointeger(pL, -1);

	LuaUtil::closeLuaFile(pL);
}

bool Player::TryTurnTo(std::string newAction)
{
	//查看是否可以从nowAction转变到newAction
	if (!CanTurnTo(newAction))
	{
		//log("this moment can`t turn %s to %s!", m_nowAction.c_str(), newAction.c_str());
		return false;
	}



	//更新状态
	m_nowAction = newAction;

	int nowActionID = getActionIDByActionType(m_nowAction);

	//对run特殊处理
	//向左跑
	if (m_nowAction == m_playerActionType.at(1))
	{
		m_armature->setScaleX(-1);
		m_armature->getAnimation()->play("run");
	}
	//向右跑
	else if (m_nowAction == m_playerActionType.at(2))
	{
		m_armature->setScaleX(1);
		m_armature->getAnimation()->play("run");
	}
	else
	{
		m_armature->getAnimation()->play(m_nowAction);
	}

	//判断战斗状态
	if (getActionIDByActionType(m_nowAction) < 5 || getActionIDByActionType(m_nowAction) == 7 || 
		getActionIDByActionType(m_nowAction) == 9)
	{
		m_battleState = NONE;
	}
	else if (getActionIDByActionType(m_nowAction) == 5
		|| getActionIDByActionType(m_nowAction) == 6)
	{
		m_battleState = ATTACK;
	}
	else if (getActionIDByActionType(m_nowAction) == 8)
	{
		m_battleState = UATTACK;
	}
	//对nowAction进行判断，是否为循环动作，否则需要等待结束
	if (nowActionID > 2 && nowActionID != 4 && nowActionID != 10)
	{
		m_isActionEnd = false;
	}
	else
	{
		m_isActionEnd = true;
	}
	return true;
}

std::string Player::getPlayerActionTypeByID(int actionID)
{
	return m_playerActionType.at(actionID);
}

int Player::getActionIDByActionType(std::string actionType)
{
	std::vector<std::string>::iterator ite = find(m_playerActionType.begin(), m_playerActionType.end(), actionType);
	int actionID = std::distance(std::begin(m_playerActionType), ite);
	return actionID;
}

RunDir Player::getRunDir()
{
	return m_runDir;
}



void Player::changeRunDir(int change)
{
	if (change > 0)
	{
		setPositive(m_runDir);
	}
	else if (change < 0)
	{
		setNegetive(m_runDir);
	}
	log("run dir: %d", m_runDir);
}

bool Player::isActionEnd()
{
	return m_isActionEnd;
}

void Player::ReLoadAction()
{
	//根据当前跑动方向更新
	switch (m_runDir)
	{
	case STAND:
		TryTurnTo(m_playerActionType.at(0));
		break;
	case RUNLEFT:
		TryTurnTo(m_playerActionType.at(1));
		break;
	case RUNRIGHT:
		TryTurnTo(m_playerActionType.at(2));
		break;
	default:
		break;
	}
}

int Player::GetTowards()
{
	return m_armature->getScaleX();
}

BattleState Player::GetBattleState()
{
	return m_battleState;
}

void Player::ForceToHurt(int damage)
{
	//处于下蹲，跳跃状态或者防守状态不播放受伤动画
	//跳跃
	if (getActionIDByActionType(m_nowAction) == 3)
	{
		m_nowHealth -= damage;
	}
	//下蹲或跳跃
	else if (getActionIDByActionType(m_nowAction) == 4 || getActionIDByActionType(m_nowAction) == 10)
	{
		m_nowHealth -= damage / 2;
	}
	else
	{
		m_nowHealth -= damage;
		m_armature->getAnimation()->play(m_playerActionType.at(9));
		m_isActionEnd = false;
	}

	m_battleState = HURT;
	m_nowAction = m_playerActionType.at(9);


	//发信更新ui信息
	SendToUpateUI();

	if (m_nowHealth <= 0)
	{
		m_nowHealth = 0;
		m_armature->getAnimation()->play(m_playerActionType.at(11));
		m_isActionEnd = false;
		m_nowAction = m_playerActionType.at(11);
	}
	log("Player: %d,Health: %d,Power: %d", m_playerID, m_nowHealth, m_nowPower);
}

float Player::GetAnimationPositionX()
{
	return m_armature->getPositionX();
}

Sprite* Player::getAnimature()
{
	return (Sprite*)m_armature;
}

Player::Player()
{
}

Player::~Player()
{

}


Player* Player::create(int id, std::string type)
{
	Player *pRet = new(std::nothrow) Player();
	if (pRet && pRet->init(id, type))
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

	//设置初始Action为stand
	m_nowAction = m_playerActionType.at(0);

	//根据playerType执行lua脚本读取动画文件
	lua_State* pL = LuaUtil::openLuaFile("luaData/testData.lua");
	lua_getglobal(pL, "getAnimationFile");
	lua_pushstring(pL, type.c_str());
	lua_call(pL, 1, 1);

	const char* c = lua_tostring(pL, -1);

	//添加动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo(c);
	m_armature = Armature::create("MatchmanAnimation");
	if (m_armature == NULL)
	{
		log("player %d animation load error!", m_playerID);
		return false;
	}
	m_armature->getAnimation()->play(m_playerActionType.at(0));
	this->setContentSize(m_armature->getContentSize());
	this->addChild(m_armature);
	
	m_isActionEnd = true;

	//假如是单数ID朝向左边
	if (m_playerID % 2 == 1)
	{
		m_armature->setScaleX(-1);
	}

	//设置帧监听
	m_armature->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(Player::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	//初始化位置
	float x, y;
	lua_getglobal(pL, "playersPos");
	lua_pushnumber(pL, m_playerID + 1);
	lua_gettable(pL, -2);
	lua_pushnumber(pL, 1);
	lua_gettable(pL, -2);
	x = lua_tonumber(pL, -1);
	lua_getglobal(pL, "playersPos");
	lua_pushnumber(pL, m_playerID + 1);
	lua_gettable(pL, -2);
	lua_pushnumber(pL, 2);
	lua_gettable(pL, -2);
	y = lua_tonumber(pL, -1);
	//this->setPosition(Vec2(x, y));


	//为了便于检测脚下砖块ID设置锚点
	//this->setAnchorPoint(Vec2(0.5F, 1.0F));

	//test
	//m_armature->setPosition(this->getPosition());
	//m_armature->setAnchorPoint(Vec2::ZERO);

	m_armature->setPosition(Vec2(x, y));
	m_armature->setAnchorPoint(Vec2(0.5F, 0.0F));
	//初始化跑动方向
	m_runDir = RunDir::STAND;

	//设置定时器
	this->scheduleUpdate();

	//设置跳跃和下落状态
	m_isJump = false;
	m_isFall = true;

	//设置初始不触地
	m_canstand = false;

	//初始健康和能量
	m_nowHealth = m_maxHealth;
	m_nowPower = 0;

	//初始化战斗状态
	m_battleState = NONE;

	//关闭lua脚本
	LuaUtil::closeLuaFile(pL);

	//增加粒子效果
	m_hurtEffect = ParticleSystemQuad::create("particleSystem/collide.plist");
	this->addChild(m_hurtEffect);
	m_hurtEffect->setAutoRemoveOnFinish(false);
	m_hurtEffect->setVisible(false);
	m_hurtEffect->setScale(0.5F);
	//设置创建时不释放粒子
	CallFunc *hideParticleFirst = CallFunc::create([&]() {
		m_hurtEffect->setVisible(true);
	});
	DelayTime *hideDuringTime = DelayTime::create(0.5F);
	this->runAction(Sequence::create(hideDuringTime, hideParticleFirst, nullptr));

	//test
	drawNode = DrawNode::create();
	this->addChild(drawNode);
	return true;
}

bool Player::CanTurnTo(std::string newAction)
{
	//新Action不在已存类型中
	if (!OtherUtil::isContain(m_playerActionType, newAction))
	{
		log("%s not exist!", newAction);
		return false;
	}

	//处于死亡状态
	if (getActionIDByActionType(newAction) == 11)
	{
		return false;
	}

	//动作没做完，处于下落 不可转换状态
	if (!m_isActionEnd || m_isFall)
	{
		return false;
	}

	//假如不够能量释放大招
	if (getActionIDByActionType(newAction) == 8 && m_nowPower < m_maxUltimateSlillNeed)
	{
		log("Not enough power!");
		return false;
	}

	//得到2个action的ID
	int nowActionID = getActionIDByActionType(m_nowAction);
	int newActionID = getActionIDByActionType(newAction);

	if (nowActionID == newActionID)
	{
		return false;
	}

	return true;
}

void Player::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(), "damage_end") == 0)
	{
		m_battleState = NONE;
	}
	if (OtherUtil::isContain(m_endEvent, evt))
	{
		m_isActionEnd = true;
		//位置改变
		//假如是瞬移需要改变位置
		if (getActionIDByActionType(m_nowAction) == 7)
		{
			m_armature->setPositionX(m_armature->getPositionX() + m_armature->getScaleX() * 56);
			checkRange();
		}
		//如果是大招也要改变位置
		else if (getActionIDByActionType(m_nowAction) == 8)
		{
			m_armature->setPositionX(m_armature->getPositionX() + m_armature->getScaleX() * 375);
			checkRange();
		}

		//使用技能增加能量
		if (getActionIDByActionType(m_nowAction) == 5 || getActionIDByActionType(m_nowAction) == 6 || getActionIDByActionType(m_nowAction) == 7)
		{
			m_nowPower += m_addPower;
			m_nowPower = m_nowPower > m_maxUltimateSlillNeed ? m_maxUltimateSlillNeed : m_nowPower;

			SendToUpateUI();
		}

		//使用大招消耗能量
		if (getActionIDByActionType(m_nowAction) == 8)
		{
			m_nowPower = 0;
			SendToUpateUI();
		
		}

		//受伤播放粒子效果
		if (getActionIDByActionType(m_nowAction) == 9)
		{
			m_hurtEffect->setPosition(m_armature->getPosition());
			m_hurtEffect->resetSystem();
		}

		//死亡停止计时器
		if (getActionIDByActionType(m_nowAction) == 11)
		{
			log("player:%d failed!", m_playerID);
			m_armature->setPositionY(m_armature->getPositionY() - 32);
			NotificationCenter::getInstance()->postNotification("resetScene", nullptr);

			//test
			//Director::getInstance()->pause();
			//auto delay = DelayTime::create(1.0F);
			//auto sendreset = CallFunc::create([&]() {
			//	NotificationCenter::getInstance()->postNotification("resetScene", nullptr);
			//});
			//this->runAction(Sequence::create(delay, sendreset, NULL));

			return;
		}

		log("Player: %d,Health: %d,Power: %d", m_playerID, m_nowHealth, m_nowPower);

		ReLoadAction();
	}
}
