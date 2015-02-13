#include "RaidTheCastleContactListener.h"
#include "PuppetScene.h"
#include "SayAction.h"

#include "Structure.h"

/*bool GameJamContactListener:: ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
	if(fixtureA->GetFilterData().maskBits == 1 || fixtureB->GetFilterData().maskBits == 1){
		// collide
	}else{
		// pass
	}
}*/

void RaidTheCastleContactListener::BeginContact(b2Contact* contact){

	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		b2Filter fA = contact->GetFixtureA()->GetFilterData();
		b2Filter fB = contact->GetFixtureB()->GetFilterData();

		if(fA.categoryBits == PuppetScene::PLAYER || fB.categoryBits == PuppetScene::PLAYER){

			if(fA.categoryBits == PuppetScene::PLAYER && fB.categoryBits == PuppetScene::PLAYER){
				// Player-Player collision
				playerPlayerContact(contact);
			}else if(fA.categoryBits == PuppetScene::ITEM || fB.categoryBits == PuppetScene::ITEM){
				// Player-Item collision
				playerItemContact(contact);
			}else if(fA.categoryBits == PuppetScene::STRUCTURE || fB.categoryBits == PuppetScene::STRUCTURE){
				// Player-Structure collision
				playerStructureContact(contact);
			}
		}
	}else{
		// do nothing
	}
}

void RaidTheCastleContactListener::playerPlayerContact(b2Contact * contact){
	b2Fixture * fxA = contact->GetFixtureA();
	b2Fixture * fxB = contact->GetFixtureB();

	PuppetCharacter * puppetA = static_cast<PuppetCharacter *>( fxA->GetBody()->GetUserData() );
    PuppetCharacter * puppetB = static_cast<PuppetCharacter *>( fxB->GetBody()->GetUserData() );

	if(puppetA != nullptr && puppetB != nullptr){
		// Stuff
	}
}

void RaidTheCastleContactListener::playerItemContact(b2Contact * contact){
	b2Fixture * fxA = contact->GetFixtureA();
	b2Fixture * fxB = contact->GetFixtureB();

	/*
	PuppetCharacter * puppet;
	Item * item;

	puppet = static_cast<PuppetCharacter *>( fxA->GetBody()->GetUserData() );
	if(puppet != nullptr){
		item = static_cast<Item *>( fxB->GetBody()->GetUserData() );
	}else{
		puppet = static_cast<PuppetCharacter *>( fxB->GetBody()->GetUserData() );
		item = static_cast<Item *>( fxA->GetBody()->GetUserData() );
	}

	if(puppet != nullptr && item != nullptr){
		// Stuff
	}*/
}

void RaidTheCastleContactListener::playerStructureContact(b2Contact * contact){
	b2Fixture * fxA = contact->GetFixtureA();
	b2Fixture * fxB = contact->GetFixtureB();

	PuppetCharacter * puppet;
	Structure * structure;

	puppet = static_cast<PuppetCharacter *>( fxA->GetBody()->GetUserData() );
	if(puppet != nullptr){
		structure = static_cast<Structure *>( fxB->GetBody()->GetUserData() );
	}else{
		puppet = static_cast<PuppetCharacter *>( fxB->GetBody()->GetUserData() );
		structure = static_cast<Structure *>( fxA->GetBody()->GetUserData() );
	}

	if(puppet != nullptr && structure != nullptr){
		// Check for cooldown

		// Fire catapult
	}
}

void RaidTheCastleContactListener::EndContact(b2Contact* contact){
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