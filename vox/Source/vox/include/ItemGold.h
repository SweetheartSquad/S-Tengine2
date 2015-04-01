#pragma once

#include <ItemSimpleWeapon.h>

class ItemGold : public ItemSimpleWeapon{
public:
	explicit ItemGold (Box2DWorld * _world);
};