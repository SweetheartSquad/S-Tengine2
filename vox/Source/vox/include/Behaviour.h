#pragma once

#include "PuppetScene.h"

#include <functional>
#include <typeinfo>
#include <vector>

class PuppetCharacter;

class Behaviour {
public:
	bool active;
	float radius;
	PuppetScene::HURLYBURLY_CATEGORY filter;
	std::vector<void *> targets;
	std::function<void(PuppetCharacter * )> functionCallback;

	Behaviour(std::function<void(PuppetCharacter *)> _callback, float _radius, PuppetScene::HURLYBURLY_CATEGORY _filter);
	~Behaviour();
};