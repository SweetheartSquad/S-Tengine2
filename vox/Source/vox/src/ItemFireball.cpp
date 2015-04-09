#pragma once

#include <ItemFireball.h>
#include <PuppetCharacterDragon.h>

ItemFireball::ItemFireball(PuppetCharacterDragon * _dragon, Box2DWorld * _world) :
	Item(true, _world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kBOUNDARY | PuppetGame::kGROUND, 0, 25),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	owner = _dragon;
	groupIndex = owner->groupIndex;
	setGroupIndex(owner->groupIndex);
}

ItemFireball::~ItemFireball(){
	PuppetCharacterDragon * dragon = dynamic_cast<PuppetCharacterDragon *>(owner);
	if(dragon == nullptr){
		throw "not a dragon!";
	}
	dragon->fireball = nullptr;
}