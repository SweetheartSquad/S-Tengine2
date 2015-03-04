#pragma once

#include "Box2DSuperSprite.h"
#include "PuppetScene.h"

class Item : public Box2DSuperSprite {
public:
	unsigned int damage;
	float handleX;
	float handleY;

	bool pickupable;

	explicit Item(bool _pickupable, Box2DWorld * _world, int16 _categoryBits = PuppetScene::kITEM, int16 _maskBits = -1, int16 _groupIndex = 0, int _damage = 0, float _handleX = 0.f, float _handleY = 0.f);
	
	~Item ();

private:

};