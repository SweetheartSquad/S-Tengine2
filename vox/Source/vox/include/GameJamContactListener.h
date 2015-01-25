#include <Box2D\Box2D.h>
#include "Character.h"

//main collision call back function
  class GameJamContactListener : public b2ContactListener
  {
	//ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	  void BeginContact(b2Contact* contact);

	  void contactWithProp(Character *& cA);
	  void contactWithCharacter(b2Contact * contact);

	  void EndContact(b2Contact* contact);
  };