#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class SelectScene : public Layer
{
public:
	CREATE_FUNC(SelectScene);
	bool init() override;
	static Scene* createScene();
private:
	void PlayerVSPlayer(Ref*, TouchEventType type);
	void PlayerVSPC(Ref*, TouchEventType type);
	void Setting(Ref*, TouchEventType type);
	void Music(Ref*, TouchEventType type);
};