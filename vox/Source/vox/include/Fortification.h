#pragma once

#include <Box2D/Box2D.h>

#include "StructureBreakable.h"

class Sprite;
class Box2DSprite;
class Box2DWorld;

class Fortification : public StructureBreakable{
public:
	Fortification(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	void takeDamage(float _damage) override;
};