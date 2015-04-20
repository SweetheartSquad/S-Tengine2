#pragma once

#include <Box2D\Box2D.h>

class LD32_Scene;

//main collision call back function
class LD32_ContactListener : public b2ContactListener{
public:
	LD32_ContactListener(LD32_Scene * _scene);

	float damageScoreMult;

	//ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	virtual void BeginContact(b2Contact* _contact) override;

	virtual void playerBumperContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _bumperFixture);
	virtual void playerBoundaryContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _boundaryFixture);
	virtual void playerEnemyContact(b2Contact* b2_contact, b2Fixture * _playerFixture, b2Fixture * _enemyFixture);
	virtual	void playerDeadZoneContact(b2Contact* b2_contact, b2Fixture* player_fixture, b2Fixture* _deadZonefixture);
	virtual	void playerMonsterContact(b2Contact* b2_contact, b2Fixture* player_fixture, b2Fixture* _monsterfixture);

	virtual void EndContact(b2Contact* _contact) override;

	LD32_Scene * scene;
};
