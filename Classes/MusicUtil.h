#pragma once
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

static class MusicUtil
{
public:
	static void preLoadAllMusic();
	static void playBackGroundMusic();
	static void playPunchOrKickMusic();
	static void playUltimatedSkillMusic();
	static void playMoveMusic();
	static void playKOMusic();
};