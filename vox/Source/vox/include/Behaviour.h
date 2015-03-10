#pragma once

#include "PuppetGame.h"

#include <functional>
#include <typeinfo>
#include <vector>

class PuppetCharacter;

class Behaviour {
public:
	bool active;
	float radius;
	PuppetGame::BOX2D_CATEGORY filter;
	std::vector<void *> targets;
	std::function<void(PuppetCharacter * )> functionCallback;

	Behaviour(std::function<void(PuppetCharacter *)> _callback, float _radius, PuppetGame::BOX2D_CATEGORY _filter);
	~Behaviour();
};