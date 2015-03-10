#pragma once

#include <PuppetGame.h>
#include <node/NodeUpdatable.h>

#include <vector>

class PuppetCharacter;
class b2Fixture;

class Behaviour abstract : public NodeUpdatable {
public:
	PuppetCharacter * source;
	bool active;
	float radius;
	PuppetGame::BOX2D_CATEGORY filter;
	
	virtual void evaluateBeginContact(b2Fixture * _target);
	virtual void evaluateEndContact(b2Fixture * _target);
	virtual void update(Step * _step) = 0;

	Behaviour(PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter);
	~Behaviour();
};