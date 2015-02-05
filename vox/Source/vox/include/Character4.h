#pragma once 

#include "GameJamCharacter.h"

class Character4 : public GameJamCharacter{
public:
	Character4(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits = -1);
	~Character4();
};