#pragma once

#include <BehaviourAttack.h>
#include <PuppetCharacter.h>
#include <Box2D\Box2D.h>
#include <Box2DSuperSprite.h>
#include <Box2DSprite.h>

BehaviourAttack::BehaviourAttack(PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter) :
	Behaviour(_source, _radius, _filter),
	forward(true)
{

}

void BehaviourAttack::evaluateBeginContact(b2Fixture * _target){
	Behaviour::evaluateBeginContact(_target);
	targets.push_back(static_cast<Box2DSuperSprite *>(_target->GetUserData()));
}

void BehaviourAttack::evaluateEndContact(b2Fixture * _target){
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


void BehaviourAttack::update(Step * _step){
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

	if(closest != nullptr){
		float t = source->torso->body->GetAngle();
		if(forward ? (t > glm::radians(60.f)) : (t < glm::radians(-60.f))){
			forward = !forward;
		}
		if(forward){
			source->targetRoll = glm::radians(-90.f);
		}else{
			source->targetRoll = glm::radians(90.f);
		}
	}
}