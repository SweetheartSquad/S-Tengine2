#pragma once 

#include "Character.h"

class Character3 : public Character{
public:
	Character3(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits = -1);
	~Character3();
};