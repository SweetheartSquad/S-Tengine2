#include <Box2D\Box2D.h>

class PuppetScene;

//main collision call back function
class RaidTheCastleContactListener : public b2ContactListener{
public:
	RaidTheCastleContactListener(PuppetScene * _scene);

//ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	virtual void BeginContact(b2Contact* contact) override;

	void playerPlayerContact(b2Contact * contact);
	void playerItemContact(b2Contact * contact, b2Fixture * playerFixture, b2Fixture * itemFixture);
	void playerStructureContact(b2Contact * contact, b2Fixture * playerFixture, b2Fixture * structureFixture);
	void playerGroundContact(b2Contact* b2_contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture);

	void structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture);

	void EndContact(b2Contact* contact) override;
  
	PuppetScene * scene;
};
