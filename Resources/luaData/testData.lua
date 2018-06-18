-- testData Lua

---------------------------------------------------------配置变量修改

maps = { "map/testMap.tmx" }			-- 地图名数组

playerNum = 2                           -- 玩家人数                         

playerType = { "Red","Green","Blue" };                 -- 玩家类型

playerAction = {  }

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

function getAnimationFile(_type)			-- 根据玩家类型得到动画文件
	-- body
	if (_type == playerType[0])
	then
		return "animation/MatchmanTest.ExportJson"
	elseif (_type == playerType[1])
	then
		return "animation/MatchmanTest.ExportJson"
	elseif (_type == playerType[2])
	then
		return "animation/MatchmanTest.ExportJson"
	else
		return nil
	end
end