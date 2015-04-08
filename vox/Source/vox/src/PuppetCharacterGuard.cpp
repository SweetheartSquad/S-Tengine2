#pragma once

#include <PuppetCharacterGuard.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

#include <RapunzelResourceManager.h>

#include <Item.h>

PuppetCharacterGuard::PuppetCharacterGuard(bool _ai, float _ghostPosition, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
	std::rand()%2 == 0 ? RapunzelResourceManager::guard1Torso : RapunzelResourceManager::guard2Torso,
	std::rand()%2 == 0 ? RapunzelResourceManager::guard1Arm : RapunzelResourceManager::guard2Arm,
	std::rand()%2 == 0 ? RapunzelResourceManager::guard1Helmet : RapunzelResourceManager::guard2Helmet
	),_ghostPosition, _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{

}

PuppetCharacterGuard::~PuppetCharacterGuard(){
}