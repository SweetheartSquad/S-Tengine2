#pragma once

#include <Structure.h>

class Box2DWorld;

class StructureBoxingGlove : public Structure{
public:
	Box2DSprite * glove;
	Box2DSprite * spring;
	void punch();
	StructureBoxingGlove(Box2DWorld * _world);
};