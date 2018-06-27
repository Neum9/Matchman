/*
	½è¼øÔ´Âë https://blog.csdn.net/yinhe888675/article/details/43952609
*/

#pragma once
#include "cocos2d.h"  
USING_NS_CC;

class Shake : public ActionInterval
{
public:
	Shake();

	static Shake* create(float d, float strength);
	static Shake* create(float d, float strength_x, float strength_y);
	bool initWithDuration(float d, float strength_x, float strength_y);

	virtual Shake* clone();
	virtual Shake* reverse(void);
	virtual void startWithTarget(Node *target);
	virtual void update(float time);
	virtual void stop(void);

protected:
	float _initial_x, _initial_y;

	float _strength_x, _strength_y;
};