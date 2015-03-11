#pragma once

#include <BehaviourPatrol.h>
#include <PuppetCharacter.h>
#include <Box2D\Box2D.h>
#include <Box2DSuperSprite.h>
#include <Box2DSprite.h>

BehaviourPatrol::BehaviourPatrol(glm::vec3 _startPoint, glm::vec3 _endPoint, PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter) :
	Behaviour(_source, _radius, _filter),
	nextWayPoint(nullptr),
	forward(true)
{
	addWayPoint(_startPoint);
	addWayPoint(_endPoint);
	nextWayPoint = &wayPoints.at(0);
}

void BehaviourPatrol::update(Step * _step){
	if(source->transform->translationVector.x){

	}
}

void BehaviourPatrol::addWayPoint(glm::vec3 _wayPoint){
	wayPoints.push_back(_wayPoint);
}

void BehaviourPatrol::addWayPoint(glm::vec3 _wayPoint, unsigned long int _index){
	_index = std::max(_index, (unsigned long int)wayPoints.size());
	wayPoints.insert(wayPoints.begin() + _index, _wayPoint);
}