#pragma once

#include "BehaviourManager.h"
#include "Behaviour.h"

#include "PuppetScene.h"
#include "PuppetCharacter.h"
#include "Box2DSprite.h"

BehaviourManager::BehaviourManager(PuppetCharacter * _target) :
	target(_target)
{
}

BehaviourManager::~BehaviourManager(){
	while(behaviours.size() > 0){
		if(behaviours.back()->fixture != nullptr){
			target->torso->body->DestroyFixture(behaviours.back()->fixture);
		}
		delete behaviours.back();
		behaviours.pop_back();
	}
}

void BehaviourManager::update(Step * _step){
	for(unsigned long int i = 0; i < behaviours.size(); ++i){
		if(behaviours.at(i)->active){
			//std::cout << behaviours.at(i)->targets.size() << std::endl;
			behaviours.at(i)->update(_step);
		}
	}
}

void BehaviourManager::addBehaviour(Behaviour * _behaviour){
	behaviours.push_back(_behaviour);
	
	b2CircleShape torsoShape;
	torsoShape.m_radius = _behaviour->radius;

	_behaviour->fixture = target->torso->body->CreateFixture(&torsoShape, 0);
	_behaviour->fixture->SetSensor(true);
	_behaviour->fixture->SetUserData(_behaviour);
	b2Filter f;
	f.categoryBits = PuppetGame::kBEHAVIOUR;
	f.maskBits = _behaviour->filter;
	f.groupIndex = target->groupIndex;
	_behaviour->fixture->SetFilterData(f);
}