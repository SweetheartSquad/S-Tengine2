#pragma once

#include <Box2D/Box2D.h>

#include "StructureBreakable.h"

class SpriteSheetAnimation;
class Sprite;
class Box2DSprite;
class Box2DWorld;

class Fortification : public StructureBreakable{
public:
	SpriteSheetAnimation * spriteSheet;
	Fortification(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~Fortification();
	void takeDamage(float _damage) override;
};