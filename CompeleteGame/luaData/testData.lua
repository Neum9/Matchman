-- testData Lua

---------------------------------------------------------配置变量修改

background = { "background/1.png","background/2.png","background/3.png","background/4.png","background/5.png","background/6.png","background/7.png","background/8.png","background/9.png","background/10.png","background/11.png","background/12.png","background/13.png","background/14.png","background/15.png","background/16.png","background/17.png"}

maps = { "map/map1.tmx","map/map3.tmx" }			-- 地图名数组

-- 地图1站立砖块ID
map1standID = { 24 }

-- 地图2站立砖块ID
map2standID = { 33 }

-- 玩家能站立的砖块编号(在Tiled上)
canStandTileID = { map1standID,map2standID }


playerNum = 2                           -- 玩家人数                         

playerType = { "Red","Blue" }                 -- 玩家类型

-- 玩家类型对应的动画文件
playerAnimationFile = { "animation/p1/MatchmanAnimation.ExportJson","animation/p2/MatchmanAnimation.ExportJson"}

-- 玩家动作列表
playerAction = { "stand","runToLeft","runToRight","jump","squat","punch","kick","move","ultimateSkill","hurt","defend","failed" }

-- 玩家1按键列表
player1Key = { "a","d","w","s","j","k","l","i","o" };

-- 玩家2按键列表
player2Key = { "left","right","up","down","1","2","3","5","6" };

-- 所有玩家按键列表
allPlayerKey = { player1Key,player2Key };

-- 按键对应动作ID（开始为1）
keyToActionID = { 2,3,4,5,6,7,8,9,11 }

-- 玩家1位置
player1Pos = { 150.0,64.0 }

-- 玩家2位置
player2Pos = { 610.0,64.0 }

-- 所有玩家位置
playersPos = { player1Pos,player2Pos }

-- 玩家每秒移动单位
playerMoveUnit = 2.0

-- 玩家每秒跳跃单位
playerJumpUnit = 12.0

-- 帧结束事件
endEvent = { "jump_end","punch_end","kick_end","move_end","ultimateSkill_end","hurt_end","defend_end","failed_end" }


-- 玩家总血量
playerHealth = 100

-- 玩家大招蓄能所需
playerultimateSkillNeed = 100

-- 各类技能伤害
punchHurt = 20
kickHurt = 20
ultimateSkillHurt = 60

-- 使用小技能增加的能量
addPower = 5

---------------------------------------------------------脚本逻辑处理

function getBackGroundNum()			        -- 得到背景图数量
	-- body
	num = -1;
	for i,v in ipairs(background) do
		num = i;
	end
	return num;
end

function getMapsLen()			        -- 得到地图数量
	-- body
	num = -1;
	for i,v in ipairs(maps) do
		num = i;
	end
	return num;
end

function getPlayerTypeNum()			        -- 得到玩家类型数量
	-- body
	num = -1;
	for i,v in ipairs(playerType) do
		num = i;
	end
	return num;
end

function getPlayerActionTypeNum()           -- 得到动作列表数量
	-- body
	num = -1;
	for i,v in ipairs(playerAction) do
		num = i;
	end
	return num;
end

function getKeyTypeNum()					-- 得到按键数量（单个玩家） 
	-- body
		num = -1;
	for i,v in ipairs(player1Key) do
		num = i;
	end
	return num;
end

function getEndEventNum()					-- 得到结束事件数量
	-- body
		num = -1;
	for i,v in ipairs(endEvent) do
		num = i;
	end
	return num;
end

function getCanStandTileNumByID(id)					-- 得到站立砖块数量by id(map order)
	-- body
		num = -1;
	for i,v in ipairs(canStandTileID[id+1]) do
		num = i;
	end
	return num;
end

function getAnimationFile(_type)			-- 根据玩家类型得到动画文件
	-- body
	for i,v in ipairs(playerType) do
		if (v == _type) then
			return playerAnimationFile[i]
		end
	end
		return nil
end

function getActionIDByKey(key)               -- 通过按键得到动作ID
	-- body
	for i,v in ipairs(allPlayerKey) do
		for j,w in ipairs(v) do
			if (key == w) then
				return keyToActionID[j] - 1  
			end
		end
	end
	return nil
end