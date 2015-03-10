#include "RaidTheCastleContactListener.h"
#include "PuppetScene.h"

#include "Scene.h"
#include "Castle.h"
#include "Boulder.h"
#include "Catapult.h"
#include "PuppetCharacter.h"
#include "Box2dWorld.h"
#include "Item.h"
#include "Box2DSprite.h"
#include "Box2DSuperSprite.h"
#include <Box2D/Box2D.h>
#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

#include "Behaviour.h"
#include <iostream>

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
	//if(contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor()){
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

		std::cout << fA.categoryBits << " | " << fB.categoryBits << std::endl;

		// if a thrown item hits the ground, make it not thrown
		if(fA.categoryBits == PuppetScene::kGROUND && fB.categoryBits == PuppetScene::kITEM){
			std::cout << "item hit ground" << std::endl;
			((Item *)contact->GetFixtureB()->GetUserData())->thrown = false;
		}else if(fB.categoryBits == PuppetScene::kGROUND && fA.categoryBits == PuppetScene::kITEM){
			std::cout << "item hit ground" << std::endl;
			((Item *)contact->GetFixtureA()->GetUserData())->thrown = false;
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
		}else{
			// structure - item?
			b2Fixture * structureFixture;
			b2Fixture * itemFixture;
			bool structure = false;
			if(fA.categoryBits == PuppetScene::kSTRUCTURE && fB.categoryBits == PuppetScene::kITEM){
				structureFixture = contact->GetFixtureA();
				itemFixture = contact->GetFixtureB();
				structure = true;
			}else if(fB.categoryBits == PuppetScene::kSTRUCTURE && fA.categoryBits == PuppetScene::kITEM){
				structureFixture = contact->GetFixtureB();
				itemFixture = contact->GetFixtureA();
				structure = true;
			}

			if(structure){
				structureItemContact(contact, structureFixture, itemFixture);
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
	PuppetCharacter * p = ((PuppetCharacter *)playerFixture->GetUserData());
	Item * item = (Item *)itemContact->GetUserData();
	if(item->thrown){
		// do some sort of damage thing here
		p->torso->applyLinearImpulseUp(500);
		if(!p->dead){
			p->deathPending = true;
		}
	}else if(p->heldItem == nullptr && !item->held){
		p->itemToPickup = item;
		// multiple players might be able to pick it up in one update
	}
}

void RaidTheCastleContactListener::playerStructureContact(b2Contact * contact, b2Fixture * playerFixture, b2Fixture * structureFixture){

	std::cout << "Player-Structure Collision" << std::endl;

	// need to actually check if the structure is the catapult
	((Catapult *)structureFixture->GetUserData())->fireCatapult();

}

void RaidTheCastleContactListener::playerGroundContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture){
	PuppetCharacter * puppet = static_cast<PuppetCharacter *>( _playerFixture->GetUserData());
	if(puppet != nullptr){
		puppet->canJump = true;
	}
}

void RaidTheCastleContactListener::structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture){
	Structure * structure = static_cast<Structure *>( _structureFixture->GetUserData() );
    Item * item = static_cast<Item *>( _itemFixture->GetUserData() );

	Castle * castle = dynamic_cast<Castle *>(structure);
	if(castle != nullptr){
		castle->damage = item->damage;
	}
}

void RaidTheCastleContactListener::EndContact(b2Contact* contact){
	
	b2Filter fA = contact->GetFixtureA()->GetFilterData();
	b2Filter fB = contact->GetFixtureB()->GetFilterData();

	b2Fixture * playerFixture = nullptr;
	if(fA.categoryBits == PuppetScene::kPLAYER){
		playerFixture = contact->GetFixtureA();
	}else if(fB.categoryBits == PuppetScene::kPLAYER){
		playerFixture = contact->GetFixtureB();
	}

	if(playerFixture != nullptr){
		PuppetCharacter * player = (PuppetCharacter *)(playerFixture->GetUserData());
		if(player != nullptr && player->deathPending){
			//player->die();
		}
	}

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

}
