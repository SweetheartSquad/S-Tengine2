#pragma once 

#include "Character.h"

class Character2 : public Character{
public:
	Character2(Box2DWorld * _world, bool _ai);
	~Character2();
};