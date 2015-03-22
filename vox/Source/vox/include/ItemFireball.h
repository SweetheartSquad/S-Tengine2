#pragma once

#include <Item.h>
class PuppetCharacterDragon;
class ItemFireball : public Item{
public:
	ItemFireball(PuppetCharacterDragon * _dragon, Box2DWorld * _world);
	~ItemFireball();
};