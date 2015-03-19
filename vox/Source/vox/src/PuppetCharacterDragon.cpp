#pragma once

#include <PuppetCharacterDragon.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <BehaviourManager.h>
#include <Box2DWorld.h>

#include <SlayTheDragonResourceManager.h>

PuppetCharacterDragon::PuppetCharacterDragon(Box2DWorld * _world, bool _ai, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		SlayTheDragonResourceManager::dragonTorso,
		SlayTheDragonResourceManager::dragonLowerWing,
		SlayTheDragonResourceManager::dragonHead,
		PuppetResourceManager::head1, 
		PuppetResourceManager::hand1,
		PuppetResourceManager::face1,
		4.0f
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), this, 10));
	behaviourManager.addBehaviour(new BehaviourAttack(this, 3, PuppetGame::kPLAYER));

	for(auto c : components) {
		(*c)->body->SetGravityScale(0.0f);
	}
}

PuppetCharacterDragon::~PuppetCharacterDragon(){
	//delete texPack;
}

void PuppetCharacterDragon::action(){
	// the castle champion isn't allowed to throw their weapon
}

void PuppetCharacterDragon::update(Step * _step){
	PuppetCharacter::update(_step);
	b2Vec2 center = torso->body->GetWorldCenter();
	torso->applyForce(torso->body->GetAngle() * -5000.0f, 0.0f, center.x, center.y);
	torso->body->SetTransform(b2Vec2(torso->body->GetPosition().x, 30.0f), torso->body->GetAngle());
	if(ai){
		if(behaviourManager.behaviours.at(1)->active){
			behaviourManager.behaviours.at(0)->active = false;
		}else{
			behaviourManager.behaviours.at(0)->active = true;
		}
	}
}