#pragma once

#include "Behaviour.h"

Behaviour::Behaviour(std::function<void(PuppetCharacter *)> _callback, float _radius, size_t _target) :
	functionCallback(_callback),
	radius(_radius),
	target(_target)
{

}

Behaviour::~Behaviour(){
}