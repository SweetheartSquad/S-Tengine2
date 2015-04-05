#pragma once

#include <Box2D/Box2D.h>

#include "StructureBreakable.h"

class Box2DSprite;
class Box2DWorld;
class SpriteSheetAnimation;

class Castle : public StructureBreakable{
public:
	SpriteSheetAnimation * spriteSheet;
	Castle(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~Castle();
	
	void takeDamage(float _damage) override;
};