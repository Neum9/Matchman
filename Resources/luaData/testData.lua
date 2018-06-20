-- testData Lua

---------------------------------------------------------配置变量修改

maps = { "map/testMap.tmx" }			-- 地图名数组

playerNum = 2                           -- 玩家人数                         

playerType = { "Red","Green","Blue" };                 -- 玩家类型

-- 玩家类型对应的动画文件
playerAnimationFile = { "animation/MatchmanTest.ExportJson","animation/MatchmanTest.ExportJson","animation/MatchmanTest.ExportJson" }

-- 玩家动作列表
playerAction = { "stand","runToLeft","runToRight","jump","squat","punch","kick","move","ultimateSkill","hurt","defend","falied" }

-- 玩家1按键列表
player1Key = { "a","d","w","s","j","k","l","i","o" };

-- 玩家2按键列表
player2Key = { "left","right","up","down","1","2","3","5","6" };

-- 所有玩家按键列表
allPlayerKey = { player1Key,player2Key };

-- 按键对应动作ID（开始为1）
keyToActionID = { 2,3,4,5,6,7,8,9,11 }

-- 玩家1位置
player1Pos = { 150.0,100.0 }

-- 玩家2位置
player2Pos = { 810.0,100.0 }

-- 所有玩家位置
playersPos = { player1Pos,player2Pos }

-- 玩家每秒移动单位
playerMoveUnit = 2.0

-- 玩家每秒跳跃单位
playerJumpUnit = 2.0

-- 帧结束事件 暂时没有做jump
endEvent = { "squat_end","punch_end","kick_end","move_end","ultimateSkill_end","hurt_end","defend_end" }

---------------------------------------------------------脚本逻辑处理

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

function getEndEventNum()					-- 得到数量
	-- body
		num = -1;
	for i,v in ipairs(endEvent) do
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