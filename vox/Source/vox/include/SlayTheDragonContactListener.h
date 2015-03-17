#pragma once

#include <PuppetContactListener.h>

class b2Contact;
class b2Fixture;
class PuppetScene;

//main collision call back function
class SlayTheDragonContactListener : public PuppetContactListener{
public:
	SlayTheDragonContactListener(PuppetScene * _scene);

//ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	virtual void BeginContact(b2Contact* contact) override;

	void playerPlayerContact(b2Contact * contact) override;
	void playerItemContact(b2Contact * contact, b2Fixture * playerFixture, b2Fixture * itemFixture) override;
	void playerStructureContact(b2Contact * contact, b2Fixture * playerFixture, b2Fixture * structureFixture) override;
	void playerGroundContact(b2Contact* b2_contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture) override;

	void structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture) override;

	void EndContact(b2Contact* contact) override;
};
