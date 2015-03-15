#pragma once

#include <Item.h>
class TextureSampler;
class ItemProjectileWeapon : public Item{
public:
	TextureSampler * projectileTex;

	explicit ItemProjectileWeapon(TextureSampler * _projectileTex, TextureSampler * _weaponTex, Box2DWorld * _world, int16 _categoryBits = PuppetGame::kITEM, int16 _maskBits = -1, int16 _groupIndex = 0, int _damage = 0, float handleX = 0.f, float handleY = 0.f);
	~ItemProjectileWeapon();

	Item * getProjectile() override;
};