#include "LoadScene.h"
#include "SelectScene.h"
#include "MusicUtil.h"

bool LoadScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//读取UI层
	Widget* UI = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("UI/LoadSceneUI/First_1.ExportJson");
	UI->setPosition(Point(0, 0));
	this->addChild(UI);

	//读取更多信息
	m_moreInfoUI = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("UI/MoreInfo/MoreInfo_1.ExportJson");
	m_moreInfoUI->setPosition(Point(0, 0));
	m_moreInfoUI->setVisible(false);
	this->addChild(m_moreInfoUI);

	//开始按钮
	auto startButton = (Button*)Helper::seekWidgetByName(UI, "start");
	startButton->addTouchEventListener(this, toucheventselector(LoadScene::Start));

	//help
	auto helpButton = (Button*)Helper::seekWidgetByName(UI, "help");
	helpButton->addTouchEventListener(this,toucheventselector(LoadScene::Help));
	//red
	auto redButton = (Button*)Helper::seekWidgetByName(UI, "red");
	redButton->addTouchEventListener(this, toucheventselector(LoadScene::Red));
	//blue
	auto blueButton = (Button*)Helper::seekWidgetByName(UI, "blue");
	blueButton->addTouchEventListener(this, toucheventselector(LoadScene::Blue));
	//more
	auto moreButton = (Button*)Helper::seekWidgetByName(UI, "more");
	moreButton->addTouchEventListener(this, toucheventselector(LoadScene::More));

	m_moreInfoUI->addTouchEventListener(this, toucheventselector(LoadScene::HideMoreInfo));

	return true;
}

Scene* LoadScene::createScene()
{
	auto layer = LoadScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}

void LoadScene::Start(Ref*, TouchEventType type)
{
	//切换到选择模式场景
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
		auto scene = SelectScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void LoadScene::Help(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
		m_moreInfoUI->setVisible(true);
	}
}

void LoadScene::Red(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
	}
}

void LoadScene::Blue(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
	}
}

void LoadScene::More(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
	}
}

void LoadScene::HideMoreInfo(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		m_moreInfoUI->setVisible(false);
	}
}
