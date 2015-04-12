#pragma once

#include <Box2D\Box2D.h>

class PuppetScene;

//main collision call back function
class PuppetContactListener : public b2ContactListener{
public:
	PuppetContactListener(PuppetScene * _scene);

	float damageScoreMult;

	//ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	virtual void BeginContact(b2Contact* _contact) override;

	virtual void playerPlayerContact(b2Contact * _contact);
	virtual void playerItemContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _itemFixture);
	virtual void playerStructureContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _structureFixture);
	virtual void playerGroundContact(b2Contact* b2_contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture);
	virtual	void playerDeadZoneContact(b2Contact* b2_contact, b2Fixture* player_fixture, b2Fixture* other_fixture);

	virtual void structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture);

	virtual void EndContact(b2Contact* _contact) override;
  
	PuppetScene * scene;
};
