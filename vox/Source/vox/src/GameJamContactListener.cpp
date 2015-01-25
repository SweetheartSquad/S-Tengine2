#include "GameJamContactListener.h"

/*bool GameJamContactListener:: ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
	if(fixtureA->GetFilterData().maskBits == 1 || fixtureB->GetFilterData().maskBits == 1){
		// collide
	}else{
		// pass
	}
}*/

void GameJamContactListener::BeginContact(b2Contact* contact){
	

	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		int16 mA = contact->GetFixtureA()->GetFilterData().maskBits;
		int16 mB = contact->GetFixtureB()->GetFilterData().maskBits;
		
		int i = 0;

		if(mA == 2 && mB == 2){
			// Both characters
		}else if((mA == 2 && mB == 4) || (mA == 4 && mB == 2)){
			// character and prop
		}else{

		}
	}else{
		// do nothing
	}

	
	
}

void GameJamContactListener::EndContact(b2Contact* contact){
	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		int16 mA = contact->GetFixtureA()->GetFilterData().maskBits;
		int16 mB = contact->GetFixtureB()->GetFilterData().maskBits;
		
		if(mA == 2 && mB == 2){
			// Both characters
		}else if((mA == 2 && mB == 4) || (mA == 4 && mB == 2)){
			// character and prop
		}else{

		}
	}else{
		// do nothing
	}
}