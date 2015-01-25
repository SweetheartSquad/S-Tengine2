#include "GameJamContactListener.h"
#include "GameJamScene.h"
/*bool GameJamContactListener:: ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
	if(fixtureA->GetFilterData().maskBits == 1 || fixtureB->GetFilterData().maskBits == 1){
		// collide
	}else{
		// pass
	}
}*/

void GameJamContactListener::BeginContact(b2Contact* contact){
	
	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		b2Filter fA = contact->GetFixtureA()->GetFilterData();
		b2Filter fB = contact->GetFixtureB()->GetFilterData();

		if(fA.categoryBits == GameJamScene::PLAYER || fB.categoryBits == GameJamScene::PLAYER){

			if(fA.categoryBits == GameJamScene::NPC || fB.categoryBits == GameJamScene::NPC){
				// Player character and character
				int i = 0;
			}else if(fA.categoryBits == GameJamScene::PROP || fB.categoryBits == GameJamScene::PROP){
				// player character and prop
				int i = 1;
			}else{

			}
		}
	}else{
		// do nothing
	}
	
}

void GameJamContactListener::EndContact(b2Contact* contact){
	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		b2Filter fA = contact->GetFixtureA()->GetFilterData();
		b2Filter fB = contact->GetFixtureB()->GetFilterData();

		if((fA.maskBits == 2 + 4 && fB.maskBits == 4) || (fA.maskBits = 4 && fB.maskBits == 2 + 4)){
			// Player character and character
			int i = 0;
		}else if((fA.maskBits == 2 + 4 && fB.maskBits == 4) || (fA.maskBits == 4 && fB.maskBits == 2 + 4)){
			// player character and prop
			int i = 1;
		}else{

		}
	}else{
		// do nothing
	}
}