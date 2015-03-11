#pragma once

#include <Behaviour.h>
#include <Box2D\Box2D.h>

Behaviour::Behaviour(PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter) :
	source(_source),
	radius(_radius),
	filter(_filter),
	active(false)
{
}

Behaviour::~Behaviour(){
}

void Behaviour::evaluateBeginContact(b2Fixture * _target){
	active = true;
}

void Behaviour::evaluateEndContact(b2Fixture * _target){
	
}