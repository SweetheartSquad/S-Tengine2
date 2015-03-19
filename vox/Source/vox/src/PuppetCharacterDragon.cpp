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
		SlayTheDragonResourceManager::dragonUpperWing,
		SlayTheDragonResourceManager::dragonHead,
		SlayTheDragonResourceManager::dragonMouth, 
		SlayTheDragonResourceManager::dragonLowerWing,
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
	
	world->b2world->DestroyJoint(handRight->body->GetJointList()->joint);
	world->b2world->DestroyJoint(handLeft->body->GetJointList()->joint);

	// right wing
	b2RevoluteJointDef rhrej;
	rhrej.bodyA = armRight->body;
	rhrej.bodyB = handRight->body;
	rhrej.localAnchorA.Set(armRight->getCorrectedWidth(), -0.2 * armRight->getCorrectedHeight());
	rhrej.localAnchorB.Set(0.6 * handRight->getCorrectedWidth(), 0.9 * handRight->getCorrectedHeight());
	rhrej.collideConnected = false;
	rhrej.enableLimit = true;
	rhrej.referenceAngle = glm::radians(0.f);
	rhrej.lowerAngle = glm::radians(-45.f);
	rhrej.upperAngle = glm::radians(0.f);
	world->b2world->CreateJoint(&rhrej);

	// left wing
	b2RevoluteJointDef lhlej;
	lhlej.bodyA = armLeft->body;
	lhlej.bodyB = handLeft->body;
	lhlej.localAnchorA.Set(-armLeft->getCorrectedWidth(), -0.2 * armLeft->getCorrectedHeight());
	lhlej.localAnchorB.Set(-0.6 * handLeft->getCorrectedWidth(), 0.9 * handLeft->getCorrectedHeight());
	lhlej.collideConnected = false;
	lhlej.enableLimit = true;
	lhlej.referenceAngle = glm::radians(0.f);
	lhlej.lowerAngle = glm::radians(0.f);
	lhlej.upperAngle = glm::radians(45.f);
	world->b2world->CreateJoint(&lhlej);
	
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