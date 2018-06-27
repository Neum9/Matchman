/*
	游戏场景类
	游戏场景逻辑管理
*/

#pragma once
#include "cocos2d.h"
#include "PlayerManager.h"
#include "GameUI.h"

USING_NS_CC;

static std::string MAP_LAYER = "mainLayer";

class GameScene : public Layer
{
public:
	GameScene();
	~GameScene();
	static Scene* createScene(PlayerManager *playerManager);
    bool init() override;
	CREATE_FUNC(GameScene);
	static bool isTileCanbeStand(float x, float y);
	static void loadMapInfo(const char* file);
	void ShowWinBGInTime(float t);
private:
	//大招背景
	Sprite* m_ultimateSkillBG;
	//胜利背景
	Sprite* m_winBG;
	//地图信息：地图名，地图数量，第几个地图
	static std::vector<std::string> m_mapNames;
	static int m_mapNum;
	static int m_mapOrder;
	static std::vector<std::vector<int>> m_canStandTileID;

	//背景图
	static std::vector<std::string> m_backgrounds;

	static TMXTiledMap *m_tileMap;

	PlayerManager *m_playerManager;

	void update(float dt) override;

	//键盘监听事件
	EventListenerKeyboard* m_listener;

	//暂停
	void Pause(Ref* pSender);

	//UI
	GameUI* m_ui;
	//收信方法 更新UI
	void UpdateUI(Ref* pSender);
	void UpdateScore(Ref* pSender);
	//收信 player失败更新场景
	void ResetScene(Ref* pSender);
	//收信 播放大招背景
	void ShowULBG(Ref* pSender);
};