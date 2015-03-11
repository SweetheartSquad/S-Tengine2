#pragma once

#include <PuppetCharacterCastleChampion.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>

PuppetCharacterCastleChampion::PuppetCharacterCastleChampion(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(_world, _categoryBits, _maskBits, _groupIndex, true),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	behaviourManager.addBehaviour(new BehaviourFollow(this, 10, PuppetGame::kPLAYER));
}

void PuppetCharacterCastleChampion::action(){
	// the castle champion isn't allowed to throw their weapon
}