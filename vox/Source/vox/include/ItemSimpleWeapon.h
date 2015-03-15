#pragma once

#include <Item.h>
class TextureSampler;
class ItemSimpleWeapon : public Item{
public:
	explicit ItemSimpleWeapon(TextureSampler * _tex, bool _singleUse, Box2DWorld * _world, int16 _categoryBits = PuppetGame::kITEM, int16 _maskBits = -1, int16 _groupIndex = 0, int _damage = 0, float handleX = 0.f, float handleY = 0.f);
	~ItemSimpleWeapon();
};