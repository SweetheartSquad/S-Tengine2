#pragma once 

#include "GameJamCharacter.h"

class Character1 : public GameJamCharacter{
public:
	Character1(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~Character1();
};