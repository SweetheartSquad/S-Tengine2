#pragma once

#include <BehaviourPatrol.h>
#include <PuppetCharacter.h>
#include <Box2D\Box2D.h>
#include <Box2DSuperSprite.h>
#include <Box2DSprite.h>

BehaviourPatrol::BehaviourPatrol(glm::vec3 _startPoint, glm::vec3 _endPoint, PuppetCharacter * _source, float _radius) :
	Behaviour(_source, _radius, (PuppetGame::BOX2D_CATEGORY)0),
	nextWayPoint(0),
	forward(true)
{
	addWayPoint(_startPoint);
	addWayPoint(_endPoint);
	active = true;
}

void BehaviourPatrol::update(Step * _step){
	float dif = wayPoints.at(nextWayPoint).x - source->rootComponent->transform->translationVector.x;
	if(abs(dif) < radius){
		if(nextWayPoint == (forward ? wayPoints.size()-1 : 0)){
			forward = !forward;
		}
		if(forward){
			++nextWayPoint;
		}else{
			--nextWayPoint;
		}
	}

	if(dif > 0){
		source->targetRoll = glm::radians(45.f);
	}else{
		source->targetRoll = glm::radians(-45.f);
	}

	//source->rootComponent->applyLinearImpulseRight(150*glm::sign(dif));

	source->jump();
	
}

void BehaviourPatrol::addWayPoint(glm::vec3 _wayPoint){
	wayPoints.push_back(_wayPoint);
}

void BehaviourPatrol::addWayPoint(glm::vec3 _wayPoint, unsigned long int _index){
	_index = std::max(_index, (unsigned long int)wayPoints.size());
	wayPoints.insert(wayPoints.begin() + _index, _wayPoint);
}