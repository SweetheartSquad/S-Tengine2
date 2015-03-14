#pragma once

#include <Item.h>

class ItemFlail : public Item{
public:
	Box2DSprite * handle;
	Box2DSprite * joint;
	Box2DSprite * head;


	explicit ItemFlail(Box2DWorld * _world, int16 _categoryBits = PuppetGame::kITEM, int16 _maskBits = -1, int16 _groupIndex = 0, int _damage = 0, float _handleX = 0.f, float _handleY = 0.f);
	~ItemFlail();
};