#pragma once

#include <PuppetCharacterDragon.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <BehaviourManager.h>
#include <Box2DWorld.h>

#include <SlayTheDragonResourceManager.h>

PuppetCharacterDragon::PuppetCharacterDragon(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(\
		SlayTheDragonResourceManager::dragonTorso,
		SlayTheDragonResourceManager::dragonLowerWing,
		SlayTheDragonResourceManager::dragonHead,
		PuppetResourceManager::head1, 
		PuppetResourceManager::hand1,
		PuppetResourceManager::face1,
		5.0f
	), true, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), this, 10));
	behaviourManager.addBehaviour(new BehaviourAttack(this, 3, PuppetGame::kPLAYER));

	for(auto c : components) {
		(*c)->body->SetGravityScale(0.0f);
	}
	//torso->body->SetGravityScale(0.0f);
}

PuppetCharacterDragon::~PuppetCharacterDragon(){
	//delete texPack;
}

void PuppetCharacterDragon::action(){
	// the castle champion isn't allowed to throw their weapon
}

void PuppetCharacterDragon::update(Step * _step){
	PuppetCharacter::update(_step);

	if(behaviourManager.behaviours.at(1)->active){
		behaviourManager.behaviours.at(0)->active = false;
	}else{
		behaviourManager.behaviours.at(0)->active = true;
	}
}