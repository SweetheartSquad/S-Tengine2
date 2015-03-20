#pragma once

#include <Behaviour.h>

#include <vector>

class PuppetCharacter;
class b2Fixture;
class Box2DSuperSprite;

class BehaviourAttack : public Behaviour {
public:
	std::vector<Box2DSuperSprite *> targets;

	bool forward;
	
	virtual void evaluateBeginContact(b2Fixture * _target) override;
	virtual void evaluateEndContact(b2Fixture * _target) override;
	virtual void update(Step * _step) override;

	BehaviourAttack(PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter);
};