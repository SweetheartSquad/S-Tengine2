#pragma once 

#include "Character.h"

class Character1 : public Character{
public:
	Character1(Box2DWorld * _world, bool _ai);
	~Character1();
};