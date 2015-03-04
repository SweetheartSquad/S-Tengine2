#pragma once

#include <functional>
#include <typeinfo>

class PuppetCharacter;

class Behaviour {
public:
	bool active;
	float radius;
	size_t target;
	std::function<void(PuppetCharacter * )> functionCallback;

	Behaviour(std::function<void(PuppetCharacter *)> _callback, float _radius, size_t _target);
	~Behaviour();
};