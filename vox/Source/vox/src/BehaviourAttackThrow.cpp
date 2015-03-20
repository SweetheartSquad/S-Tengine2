#pragma once

#include <BehaviourAttackThrow.h>
#include <PuppetCharacter.h>
#include <Box2D\Box2D.h>
#include <Box2DSuperSprite.h>
#include <Box2DSprite.h>

BehaviourAttackThrow::BehaviourAttackThrow(bool _alwaysFire, PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter) :
	BehaviourAttack(_source, _radius, _filter),
	alwaysFire(_alwaysFire)
{

}

void BehaviourAttackThrow::evaluateBeginContact(b2Fixture * _target){
	if (alwaysFire){
		active = true;
	}else{
		Behaviour::evaluateBeginContact(_target);
		targets.push_back(static_cast<Box2DSuperSprite *>(_target->GetUserData()));
	}
}

void BehaviourAttackThrow::evaluateEndContact(b2Fixture * _target){
	if (alwaysFire){
		active = true;
	}else{
		Behaviour::evaluateEndContact(_target);
		for (unsigned long int i = targets.size(); i > 0; --i){
			if (targets.at(i - 1) == _target->GetUserData()){
				targets.erase(targets.begin() + (i - 1));
				//break;
			}
		}
		if (targets.size() == 0){
			active = false;
		}
	}
}

void BehaviourAttackThrow::update(Step * _step){
	source->action();
}