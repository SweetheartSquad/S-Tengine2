#pragma once 

#include "GameJamCharacter.h"

class Character3 : public GameJamCharacter{
public:
	Character3(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~Character3();
};