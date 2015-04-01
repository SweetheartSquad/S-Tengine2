#pragma once

#include <ItemGold.h>
#include <Box2DWorld.h>
#include <Box2DSprite.h>
#include <RapunzelResourceManager.h>
#include <PuppetGame.h>

ItemGold::ItemGold(Box2DWorld * _world) :
	ItemSimpleWeapon(RapunzelResourceManager::goldBrick, false, _world, PuppetGame::kITEM, PuppetGame::kGROUND | PuppetGame::kPLAYER),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable()
{

}