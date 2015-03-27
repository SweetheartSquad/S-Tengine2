#pragma once

#include <Box2D/Box2D.h>

#include "StructureBreakable.h"

class Box2DSprite;
class Box2DWorld;

class Castle : public StructureBreakable{
public:
	Castle(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	
	void takeDamage(float _damage) override;
};