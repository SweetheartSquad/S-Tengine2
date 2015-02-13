#include <Box2D\Box2D.h>
#include "Character.h"

//main collision call back function
  class RaidTheCastleContactListener : public b2ContactListener
  {
	//ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	  void BeginContact(b2Contact* contact);

	  void playerPlayerContact(b2Contact * contact);
	  void playerItemContact(b2Contact * contact);
	  void playerStructureContact(b2Contact * contact);

	  void EndContact(b2Contact* contact);
  };