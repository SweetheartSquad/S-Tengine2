#pragma once 

#include "Character.h"

class Character2 : public Character{
public:
	Character2(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits = -1);
	~Character2();
};