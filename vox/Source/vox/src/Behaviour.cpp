#pragma once

#include "Behaviour.h"

Behaviour::Behaviour(std::function<void(PuppetCharacter *)> _callback, float _radius, PuppetScene::HURLYBURLY_CATEGORY _filter) :
	functionCallback(_callback),
	radius(_radius),
	filter(_filter),
	active(false)
{
}

Behaviour::~Behaviour(){
}