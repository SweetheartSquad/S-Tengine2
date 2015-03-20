#pragma once

#include <ItemFireball.h>
#include <PuppetCharacterDragon.h>

ItemFireball::ItemFireball(PuppetCharacterDragon * _dragon, Box2DWorld * _world) :
Item(true, _world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	dragon(_dragon)
{
	groupIndex = dragon->groupIndex;
	setGroupIndex(dragon->groupIndex);
}

ItemFireball::~ItemFireball(){
	dragon->fireball = nullptr;
}