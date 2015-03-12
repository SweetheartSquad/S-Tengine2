#pragma once

#include <PuppetCharacterCastleChampion.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

PuppetCharacterCastleChampion::PuppetCharacterCastleChampion(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	//texPack(),
	PuppetCharacter(new PuppetTexturePack(
		RaidTheCastleResourceManager::knightBlueTorso,
		RaidTheCastleResourceManager::knightGreenArm,
		RaidTheCastleResourceManager::knightRedHelmet
	), true, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), this, 10));
	//behaviourManager.addBehaviour(new BehaviourFollow(this, 10, PuppetGame::kPLAYER));
}

PuppetCharacterCastleChampion::~PuppetCharacterCastleChampion(){
	//delete texPack;
}

void PuppetCharacterCastleChampion::action(){
	// the castle champion isn't allowed to throw their weapon
}