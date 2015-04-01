#pragma once

#include <Box2D/Box2D.h>

#include "StructureBreakable.h"

class Box2DSprite;
class Box2DWorld;

class Tower : public Structure{
public:

	Tower(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
};