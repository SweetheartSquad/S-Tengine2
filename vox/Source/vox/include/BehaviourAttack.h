#pragma once

#include <Behaviour.h>

#include <vector>

class PuppetCharacter;
class b2Fixture;
class Box2DSuperSprite;

class BehaviourFollow : public Behaviour {
public:
	std::vector<Box2DSuperSprite *> targets;
	
	void evaluateBeginContact(b2Fixture * _target) override;
	void evaluateEndContact(b2Fixture * _target) override;
	void update(Step * _step) override;

	BehaviourFollow(PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter);
};