#pragma once

#include "Character.h"

class TestCharacter : public Character
{
public:
	TestCharacter(Box2DWorld * _world);
	~TestCharacter();
};