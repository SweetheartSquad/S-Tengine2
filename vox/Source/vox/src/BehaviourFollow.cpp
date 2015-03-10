#pragma once

#include <BehaviourFollow.h>
#include <PuppetCharacter.h>
#include <Box2D\Box2D.h>
#include <Box2DSuperSprite.h>
#include <Box2DSprite.h>

BehaviourFollow::BehaviourFollow(PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter) :
	Behaviour(_source, _radius, _filter)
{

}

void BehaviourFollow::evaluateBeginContact(b2Fixture * _target){
	Behaviour::evaluateBeginContact(_target);
	targets.push_back(static_cast<Box2DSuperSprite *>(_target->GetUserData()));
}
void BehaviourFollow::evaluateEndContact(b2Fixture * _target){
	Behaviour::evaluateEndContact(_target);
	for(unsigned long int i = targets.size(); i > 0; --i){
		if(targets.at(i-1) == _target->GetUserData()){
			targets.erase (targets.begin()+(i-1));
			//break;
		}
	}
	if(targets.size() == 0){
		active = false;
	}
}


void BehaviourFollow::update(Step * _step){
	Box2DSuperSprite * closest = nullptr;
	float closestDist = 99999999.f;
	glm::vec3 closestDir(0,0,0);
	for(Box2DSuperSprite * t : targets){
		glm::vec3 dir = source->rootComponent->getPos(false) - t->rootComponent->getPos(false);
		float distance = dir.x * dir.x + dir.y * dir.y;
		if(distance < closestDist){
			closestDist = distance;
			closest = t;
			closestDir = dir;
		}
	}

	if(closest != nullptr && closestDist > radius/2.f){
		closestDir = glm::normalize(closestDir);
		float speed = 25.f;
		source->torso->applyLinearImpulseLeft(closestDir.x*speed);
		//source->torso->applyLinearImpulseUp(closestDir.y*speed);

		// actual lean/jump stuff should be here instead
	}
}