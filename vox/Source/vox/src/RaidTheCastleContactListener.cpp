#include "RaidTheCastleContactListener.h"
#include "PuppetScene.h"

#include "Scene.h"
#include "Catapult.h"
#include "PuppetCharacter.h"
#include "Box2dWorld.h"
#include "Item.h"
#include "Box2DSprite.h"
#include <Box2D/Box2D.h>
#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

#include "Behaviour.h"

/*bool GameJamContactListener:: ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
	if(fixtureA->GetFilterData().maskBits == 1 || fixtureB->GetFilterData().maskBits == 1){
		// collide
	}else{
		// pass
	}
}*/

RaidTheCastleContactListener::RaidTheCastleContactListener(PuppetScene * _scene) :
	scene(_scene)
{
}

void RaidTheCastleContactListener::BeginContact(b2Contact* contact){
	if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
		b2Filter fA = contact->GetFixtureA()->GetFilterData();
		b2Filter fB = contact->GetFixtureB()->GetFilterData();
		b2Fixture * playerFixture = nullptr;
		b2Fixture * otherFixture = nullptr;
		if(fA.categoryBits == PuppetScene::kPLAYER){
			playerFixture = contact->GetFixtureA();
			otherFixture = contact->GetFixtureB();
		}else if(fB.categoryBits == PuppetScene::kPLAYER){
			playerFixture = contact->GetFixtureB();
			otherFixture = contact->GetFixtureA();
		}

		if(playerFixture != nullptr){
			if(fA.categoryBits == PuppetScene::kPLAYER && fB.categoryBits == PuppetScene::kPLAYER){
				// Player-Player collision
				playerPlayerContact(contact);
			}else if(fA.categoryBits == PuppetScene::kITEM || fB.categoryBits == PuppetScene::kITEM){
				// Player-Item collision
				playerItemContact(contact, playerFixture, otherFixture);
			}else if(fA.categoryBits == PuppetScene::kSTRUCTURE || fB.categoryBits == PuppetScene::kSTRUCTURE){
				// Player-Structure collision
				playerStructureContact(contact, playerFixture, otherFixture);
			}else if(fA.categoryBits == PuppetScene::kGROUND || fB.categoryBits == PuppetScene::kGROUND){
				// Player-Ground collision
				playerGroundContact(contact, playerFixture, otherFixture);
			}
		}

		// behaviour stuff
		b2Fixture * behaviourFixture = nullptr;

		if(fA.categoryBits == PuppetScene::kBEHAVIOUR){
			behaviourFixture = contact->GetFixtureA();
			otherFixture = contact->GetFixtureB();
		}else if(fB.categoryBits == PuppetScene::kBEHAVIOUR){
			behaviourFixture = contact->GetFixtureB();
			otherFixture = contact->GetFixtureA();
		}

		if(behaviourFixture != nullptr){
			Behaviour * b = ((Behaviour *)behaviourFixture->GetUserData());
			b->targets.push_back(otherFixture->GetUserData());
			b->active = true;
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
	
	//((PuppetCharacter *)playerFixture->GetUserData())->pickupItem(((Item *)itemContact->GetUserData());
		((PuppetCharacter *)playerFixture->GetUserData())->itemToPickup = (Item *)itemContact->GetUserData();

	

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
		}else if(fA.categoryBits == PuppetScene::kPLAYER && fB.categoryBits == PuppetScene::kGROUND || fB.categoryBits == PuppetScene::kPLAYER && fA.categoryBits == PuppetScene::kGROUND){
			PuppetCharacter * puppet;
			if(contact->GetFixtureA()->GetFilterData().categoryBits == PuppetScene::kPLAYER){
				puppet = static_cast<PuppetCharacter *>( contact->GetFixtureA()->GetUserData());
				if(puppet != nullptr){
					puppet->canJump = false;
				}
			}else if(contact->GetFixtureB()->GetFilterData().categoryBits == PuppetScene::kPLAYER){
				puppet = static_cast<PuppetCharacter *>( contact->GetFixtureB()->GetUserData());
				if(puppet != nullptr){
					puppet->canJump = false;
				}
			}
		}


		// behaviour stuff
		b2Fixture * behaviourFixture = nullptr;
		b2Fixture * otherFixture = nullptr;

		if(fA.categoryBits == PuppetScene::kBEHAVIOUR){
			behaviourFixture = contact->GetFixtureA();
			otherFixture = contact->GetFixtureB();
		}else if(fB.categoryBits == PuppetScene::kBEHAVIOUR){
			behaviourFixture = contact->GetFixtureB();
			otherFixture = contact->GetFixtureA();
		}

		if(behaviourFixture != nullptr){
			Behaviour * b = ((Behaviour *)behaviourFixture->GetUserData());
			for(unsigned long int i = b->targets.size(); i > 0; --i){
				if(b->targets.at(i-1) == otherFixture->GetUserData()){
					b->targets.erase (b->targets.begin()+(i-1));
					//break;
				}
			}
			if(b->targets.size() == 0){
				b->active = false;
			}
		}

	}else{
		// do nothing
	}
}