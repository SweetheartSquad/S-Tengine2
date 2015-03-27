#pragma once

#include <ItemProjectileWeapon.h>

class PuppetCharacterDragon;

class ItemFireballLauncher : public ItemProjectileWeapon{
public:
	ItemFireballLauncher(PuppetCharacterDragon * _dragon, Box2DWorld * _world);

	virtual Item * getProjectile() override;
};