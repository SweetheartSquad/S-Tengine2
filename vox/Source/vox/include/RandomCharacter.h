#pragma once

#include "GameJamCharacter.h"

class RandomCharacter : public GameJamCharacter{
public:
	RandomCharacter(Box2DWorld * _world, bool _ai);
	~RandomCharacter();
};