#include "MusicUtil.h"


void MusicUtil::preLoadAllMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bg.mp3");

	SimpleAudioEngine::getInstance()->preloadEffect("music/KO.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/move.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/punch&kick.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("music/UltimateSkills.mp3");
}

void MusicUtil::playBackGroundMusic()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bg.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1F);
}

void MusicUtil::playPunchOrKickMusic()
{
	SimpleAudioEngine::getInstance()->playEffect("music/punch&kick.wav");
}

void MusicUtil::playUltimatedSkillMusic()
{
	SimpleAudioEngine::getInstance()->playEffect("music/UltimateSkills.mp3");
}

void MusicUtil::playMoveMusic()
{
	SimpleAudioEngine::getInstance()->playEffect("music/move.mp3");
}

void MusicUtil::playKOMusic()
{
	SimpleAudioEngine::getInstance()->playEffect("music/KO.wav");
}

