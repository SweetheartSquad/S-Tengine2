#include "RaidTheCastleContactListener.h"
#include "PuppetScene.h"

#include "Scene.h"
#include "Catapult.h"
#include "PuppetCharacter.h"
#include "Item.h"
#include "Box2DSprite.h"
#include <Box2D/Box2D.h>
#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

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
		b2Fixture * playerFixture;
		b2Fixture * otherFixture;
		bool player = false;
		if(fA.categoryBits == PuppetScene::PLAYER){
			playerFixture = contact->GetFixtureA();
			otherFixture = contact->GetFixtureB();
			player = true;
		}else if(fB.categoryBits == PuppetScene::PLAYER){
			playerFixture = contact->GetFixtureB();
			otherFixture = contact->GetFixtureA();
			player = true;
		}

		if(player){
			if(fA.categoryBits == PuppetScene::PLAYER && fB.categoryBits == PuppetScene::PLAYER){
				// Player-Player collision
				playerPlayerContact(contact);
			}else if(fA.categoryBits == PuppetScene::ITEM || fB.categoryBits == PuppetScene::ITEM){
				// Player-Item collision
				playerItemContact(contact, playerFixture, otherFixture);
			}else if(fA.categoryBits == PuppetScene::STRUCTURE || fB.categoryBits == PuppetScene::STRUCTURE){
				// Player-Structure collision
				playerStructureContact(contact, playerFixture, otherFixture);
			}else if(fA.categoryBits == PuppetScene::GROUND || fB.categoryBits == PuppetScene::GROUND){
				// Player-Ground collision
				playerGroundContact(contact, playerFixture, otherFixture);
			}
		}
	}else{
		// do nothing
	}
}

void RaidTheCastleContactListener::playerPlayerContact(b2Contact * contact){
	b2Fixture * fxA = contact->GetFixtureA();
	b2Fixture * fxB = contact->GetFixtureB();

	PuppetCharacter * puppetA = reinterpret_cast<PuppetCharacter *>( fxA->GetBody()->GetUserData() );
    PuppetCharacter * puppetB = reinterpret_cast<PuppetCharacter *>( fxB->GetBody()->GetUserData() );

	if(puppetA != nullptr && puppetB != nullptr){
		// Stuff
	}
}

void RaidTheCastleContactListener::playerItemContact(b2Contact * contact, b2Fixture * playerFixture, b2Fixture * itemContact){
	std::cout << "Player-Item Collision" << std::endl;
	/*puppet = static_cast<PuppetCharacter *>( fxA->GetBody()->GetUserData() );
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

void RaidTheCastleContactListener::playerStructureContact(b2Contact * contact, b2Fixture * playerFixture, b2Fixture * structureFixture){
	
	std::cout << "Player-Structure Collision" << std::endl;
	
	// need to actually check if the structure is the catapult
	((Catapult *)structureFixture->GetUserData())->fireCatapult();

	/*std::cout << typeid (fxA->GetUserData()).name() << std::endl;
	if(typeid (fxA->GetUserData()) == typeid (Catapult *)){
		((Catapult *)fxA->GetUserData())->fireCatapult();
	std::cout << "FIRE THE CATAPULT!" << std::endl;
	}
	if(typeid (fxB->GetUserData()) == typeid (Catapult *)){
		((Catapult *)fxB->GetUserData())->fireCatapult();
	std::cout << "FIRE THE CATAPULT!" << std::endl;
	}*/
	/*PuppetCharacter * puppet;
	Catapult * catapult;

	puppet = static_cast<PuppetCharacter *>( fxA->GetUserData() );
	if(puppet != nullptr){
		catapult = static_cast<Catapult *>( fxB->GetUserData() );
	}else{
		puppet = static_cast<PuppetCharacter *>( fxB->GetUserData() );
		catapult = static_cast<Catapult *>( fxA->GetUserData() );
	}
	
	if(puppet != nullptr && catapult != nullptr){
		catapult->fireCatapult();
	}*/
}

void RaidTheCastleContactListener::playerGroundContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture){
	PuppetCharacter * puppet = static_cast<PuppetCharacter *>( _playerFixture->GetUserData());
	if(puppet != nullptr){
		puppet->canJump = true;
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
		}else if(fA.categoryBits == PuppetScene::PLAYER && fB.categoryBits == PuppetScene::GROUND || fB.categoryBits == PuppetScene::PLAYER && fA.categoryBits == PuppetScene::GROUND){
			PuppetCharacter * puppet;
			if(contact->GetFixtureA()->GetFilterData().categoryBits == PuppetScene::PLAYER){
				puppet = static_cast<PuppetCharacter *>( contact->GetFixtureA()->GetUserData());
				if(puppet != nullptr){
					puppet->canJump = false;
				}
			}else if(contact->GetFixtureB()->GetFilterData().categoryBits == PuppetScene::PLAYER){
				puppet = static_cast<PuppetCharacter *>( contact->GetFixtureB()->GetUserData());
				if(puppet != nullptr){
					puppet->canJump = false;
				}
			}
		}
	}else{
		// do nothing
	}
}