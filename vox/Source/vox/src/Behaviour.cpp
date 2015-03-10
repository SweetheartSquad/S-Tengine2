#pragma once

#include "Behaviour.h"

Behaviour::Behaviour(std::function<void(PuppetCharacter *)> _callback, float _radius, PuppetGame::BOX2D_CATEGORY _filter) :
	functionCallback(_callback),
	radius(_radius),
	filter(_filter),
	active(false)
{
}

Behaviour::~Behaviour(){
}