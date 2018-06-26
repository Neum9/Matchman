#include "GameUI.h"

bool GameUI::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto UI = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("UI/Matchman_blood&energy_UI_1.ExportJson");
	UI->setPosition(Point(0, 0));
	this->addChild(UI);
	//读取组件
	m_healths.push_back((ImageView*)Helper::seekWidgetByName(UI, "blood_bar1"));
	m_healths.push_back((ImageView*)Helper::seekWidgetByName(UI, "blood_bar2"));

	m_powers.push_back((ImageView*)Helper::seekWidgetByName(UI, "energy_bar1"));
	m_powers.push_back((ImageView*)Helper::seekWidgetByName(UI, "energy_bar2"));

	m_maxShows.push_back((ImageView*)Helper::seekWidgetByName(UI, "energy_max1"));
	m_maxShows.push_back((ImageView*)Helper::seekWidgetByName(UI, "energy_max2"));

	auto pauseButton = (Button*)Helper::seekWidgetByName(UI, "pause");
	pauseButton->addTouchEventListener(this, toucheventselector(GameUI::PauseGame));
	
	//添加vs和得分标签
	auto tips_vs = Label::createWithBMFont("fonts/futura-48.fnt", "VS");
	tips_vs->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 6 / 7));
	this->addChild(tips_vs);

	auto score1 = Label::createWithBMFont("fonts/futura-48.fnt", "0");
	score1->setPosition(Vec2(visibleSize.width * 7 / 16, visibleSize.height * 6 / 7));
	this->addChild(score1);
	score1->setTag(18);

	auto score2 = Label::createWithBMFont("fonts/futura-48.fnt", "0");
	score2->setPosition(Vec2(visibleSize.width * 9 / 16, visibleSize.height * 6 / 7));
	this->addChild(score2);
	score2->retain();
	score2->setTag(19);

	//初始化
	m_maxShows.at(0)->setVisible(false);
	m_maxShows.at(1)->setVisible(false);

	m_powers.at(0)->setScaleX(0);
	m_powers.at(1)->setScaleX(0);

	return true;
}

void GameUI::SetHealthByID(int id, int health)
{
	auto action = ScaleTo::create(0.5F, health / 100.0F, 1.0F);
	m_healths.at(id)->runAction(action);
}

void GameUI::SetPowerByID(int id, int power)
{
	auto action = ScaleTo::create(0.5F, power / 100.0F, 1.0F);
	m_powers.at(id)->runAction(action);
	if (power == 100)
	{
		m_maxShows.at(id)->setVisible(true);
	}
	else
	{
		m_maxShows.at(id)->setVisible(false);
	}
}

void GameUI::SetScoreByID(int id,int score)
{
	this->getChildByTag<Label*>(18 + id)->setString(Value(score).asString());
}

void GameUI::PauseGame(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		NotificationCenter::getInstance()->postNotification("pause", nullptr);
	}
}

