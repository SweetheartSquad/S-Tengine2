#pragma once

#include <Structure.h>

class Lever :public Structure
{
public:
	bool ready;
	bool triggering;
	bool pullingLever;
	float cooldownCnt;
	
	Box2DSprite * base;
	Box2DSprite * handle;

	Lever(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex);
	~Lever();

	virtual void pullLever();

private:

};