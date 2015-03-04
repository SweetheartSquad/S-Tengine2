#pragma once

#include "BehaviourManager.h"
#include "Behaviour.h"

BehaviourManager::BehaviourManager(PuppetCharacter * _target) :
	target(_target)
{
}

BehaviourManager::~BehaviourManager(){
	while(behaviours.size() > 0){
		delete behaviours.back();
		behaviours.pop_back();
	}
}

void BehaviourManager::update(Step * _step){
	for(unsigned long int i = 0; i < behaviours.size(); ++i){
		behaviours.at(i)->functionCallback(target);
	}
}

void BehaviourManager::addBehaviour(Behaviour * _behaviour){
	behaviours.push_back(_behaviour);
}