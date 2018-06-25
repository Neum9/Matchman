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

