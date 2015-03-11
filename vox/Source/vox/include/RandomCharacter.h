#pragma once

#include "GameJamCharacter.h"

class RandomCharacter : public GameJamCharacter{
public:
	RandomCharacter(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits, int16 _groupIndex);
	~RandomCharacter();
};