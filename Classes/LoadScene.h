#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class LoadScene : public Layer
{
public:
	CREATE_FUNC(LoadScene);
	bool init() override;
	static Scene* createScene();
private:
	void Start(Ref*, TouchEventType type);
	void Help(Ref*, TouchEventType type);
	void Red(Ref*, TouchEventType type);
	void Blue(Ref*, TouchEventType type);
	void More(Ref*, TouchEventType type);
	void HideMoreInfo(Ref*, TouchEventType type);
	Widget* m_moreInfoUI;
};