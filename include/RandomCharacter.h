#pragma once

#include "Character.h"

class RandomCharacter : public Character{
public:
	RandomCharacter(Box2DWorld * _world, bool _ai);
	~RandomCharacter();
};