#pragma once

#include <Item.h>
class PuppetCharacterDragon;
class ItemFireball : public Item{
public:
	PuppetCharacterDragon * dragon;
	ItemFireball(PuppetCharacterDragon * _dragon, Box2DWorld * _world);
	~ItemFireball();
};