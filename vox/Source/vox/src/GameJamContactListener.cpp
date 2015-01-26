#include "GameJamContactListener.h"
#include "GameJamScene.h"
#include "SayAction.h"

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
				contactWithCharacter(contact);
			}else if(fA.categoryBits == GameJamScene::PROP || fB.categoryBits == GameJamScene::PROP){
				// player character and prop
				int i = 1;
				//contactWithProp(dynamic_cast<Character *>(fxA->GetBody()->GetUserData()) );
			}else{

			}
		}
	}else{
		// do nothing
	}
	
}

void GameJamContactListener::contactWithCharacter(b2Contact * contact){
	b2Fixture * fxA = contact->GetFixtureA();
	b2Fixture * fxB = contact->GetFixtureB();

	Character* cA = static_cast<Character*>( fxA->GetBody()->GetUserData() );
    Character* cB = static_cast<Character*>( fxB->GetBody()->GetUserData() );

	SayAction(cA, "Ow!", 100.0f);
	SayAction(cB, "Ow!", 100.0f);
}

void GameJamContactListener::contactWithProp(Character *& cA){
	//if (static_cast<Character *>(contact->getFi))
	//SayAction(cA, "Ow!", 100.0f);
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