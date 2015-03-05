#pragma once

#include <vector>

#include "node\NodeUpdatable.h"

class Behaviour;
class PuppetCharacter;

class BehaviourManager : public NodeUpdatable{
public:
	PuppetCharacter * target;

	std::vector<Behaviour * > behaviours;

	void addBehaviour(Behaviour * _behaviour);

	void update(Step * _step) override;

	BehaviourManager(PuppetCharacter * _target);
	~BehaviourManager();
};