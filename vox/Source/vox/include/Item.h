#pragma once

#include "Box2DSuperSprite.h"

class Item :public Box2DSuperSprite {
public:
	unsigned int damage;
	float handleX;
	float handleY;

	explicit Item(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int _damage = 0, float _handleX = 0.f, float _handleY = 0.f);
	
	~Item ();

private:

};