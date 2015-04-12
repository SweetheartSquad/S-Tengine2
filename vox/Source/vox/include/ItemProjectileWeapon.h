#pragma once

#include <Item.h>
class TextureSampler;
class ItemProjectileWeapon : public Item{
public:
	TextureSampler * projectileTex;
	float projectileDamage;

	explicit ItemProjectileWeapon(TextureSampler * _projectileTex, TextureSampler * _weaponTex, Box2DWorld * _world, int16 _categoryBits = PuppetGame::kITEM, int16 _maskBits = -1, int16 _groupIndex = 0, float _damage = 0, float _projectileDamage = 0, float handleX = 0.f, float handleY = 0.f);
	~ItemProjectileWeapon();

    virtual Item * getProjectile(bool _forceDrop) override;
};