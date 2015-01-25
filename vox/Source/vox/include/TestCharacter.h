#pragma once

#include "Character.h"

class TestCharacter : public Character
{
public:
	TestCharacter(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits = -1);
	~TestCharacter();
};