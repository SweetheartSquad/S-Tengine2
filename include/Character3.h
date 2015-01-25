#pragma once 

#include "Character.h"

class Character3 : public Character{
public:
	Character3(Box2DWorld * _world, bool _ai);
	~Character3();
};