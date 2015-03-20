#pragma once

#include <BehaviourAttack.h>

#include <vector>

class PuppetCharacter;
class b2Fixture;
class Box2DSuperSprite;

class BehaviourAttackThrow : public BehaviourAttack {
public:
	bool alwaysFire;
	void update(Step * _step) override;

	virtual void evaluateBeginContact(b2Fixture * _target) override;
	virtual void evaluateEndContact(b2Fixture * _target) override;

	BehaviourAttackThrow(bool _alwaysFire, PuppetCharacter * _source, float _radius, PuppetGame::BOX2D_CATEGORY _filter);
};