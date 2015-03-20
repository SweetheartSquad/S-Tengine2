#include "PuppetContactListener.h"
#include "PuppetScene.h"

#include "Scene.h"
#include "PuppetCharacter.h"
#include "Box2dWorld.h"
#include "Item.h"
#include "Box2DSprite.h"
#include "Box2DSuperSprite.h"
#include <Box2D/Box2D.h>
#include <SoundManager.h>

#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

#include "Behaviour.h"

/*bool GameJamContactListener:: ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
	if(fixtureA->GetFilterData().maskBits == 1 || fixtureB->GetFilterData().maskBits == 1){
		// collide
	}else{
		// pass
	}
}*/

PuppetContactListener::PuppetContactListener(PuppetScene * _scene) :
	scene(_scene)
{
	_scene->world->b2world->SetContactListener(this);
}

void PuppetContactListener::BeginContact(b2Contact * _contact){
	b2Filter fA = _contact->GetFixtureA()->GetFilterData();
	b2Filter fB = _contact->GetFixtureB()->GetFilterData();
	b2Fixture * playerFixture = nullptr;
	b2Fixture * otherFixture = nullptr;
	if(fA.categoryBits == PuppetGame::kPLAYER){
		playerFixture = _contact->GetFixtureA();
		otherFixture = _contact->GetFixtureB();
	}else if(fB.categoryBits == PuppetGame::kPLAYER){
		playerFixture = _contact->GetFixtureB();
		otherFixture = _contact->GetFixtureA();
	}

	//std::cout << fA.categoryBits << " | " << fB.categoryBits << std::endl;

	if(playerFixture != nullptr){
		if((fA.categoryBits & PuppetGame::kPLAYER) != 0 && (fB.categoryBits & PuppetGame::kPLAYER) != 0){
			// Player-Player collision
			playerPlayerContact(_contact);
		}else if((fA.categoryBits & PuppetGame::kITEM) != 0 || (fB.categoryBits & PuppetGame::kITEM) != 0){
			// Player-Item collision
			//std::cout << fA.categoryBits << "|" << fB.categoryBits << std::endl;
			playerItemContact(_contact, playerFixture, otherFixture);
		}else if((fA.categoryBits & PuppetGame::kSTRUCTURE) != 0 || (fB.categoryBits & PuppetGame::kSTRUCTURE) != 0){
			// Player-Structure collision
			playerStructureContact(_contact, playerFixture, otherFixture);
		}else if((fA.categoryBits & PuppetGame::kGROUND) != 0 || (fB.categoryBits & PuppetGame::kGROUND) != 0){
			// Player-Ground collision
			playerGroundContact(_contact, playerFixture, otherFixture);
		}
	}else{
		// neither of the fixtures is a player

		// if a thrown item hits the ground, make it not thrown and able to collide regularly
		if((fA.categoryBits & PuppetGame::kGROUND) != 0 && (fB.categoryBits & PuppetGame::kITEM) != 0){
			Item * item = static_cast<Item *>(_contact->GetFixtureB()->GetUserData());
			item->hitGround();
		}else if((fB.categoryBits & PuppetGame::kGROUND) != 0 && (fA.categoryBits & PuppetGame::kITEM) !=0){
			Item * item = static_cast<Item *>(_contact->GetFixtureA()->GetUserData());
			item->hitGround();
		}

		// structure - item?
		b2Fixture * structureFixture;
		b2Fixture * itemFixture;
		bool structure = false;
		if((fA.categoryBits & PuppetGame::kSTRUCTURE) != 0 && (fB.categoryBits & PuppetGame::kITEM) != 0){
			structureFixture = _contact->GetFixtureA();
			itemFixture = _contact->GetFixtureB();
			structure = true;
		}else if((fB.categoryBits & PuppetGame::kSTRUCTURE) != 0 && (fA.categoryBits & PuppetGame::kITEM) != 0){
			structureFixture = _contact->GetFixtureB();
			itemFixture = _contact->GetFixtureA();
			structure = true;
		}

		if(structure){
			structureItemContact(_contact, structureFixture, itemFixture);
		}
	}

	// behaviour stuff
	b2Fixture * behaviourFixture = nullptr;

	if((fA.categoryBits & PuppetGame::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureA();
		otherFixture = _contact->GetFixtureB();
	}else if((fB.categoryBits & PuppetGame::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureB();
		otherFixture = _contact->GetFixtureA();
	}

	if(behaviourFixture != nullptr){
		static_cast<Behaviour *>(behaviourFixture->GetUserData())->evaluateBeginContact(otherFixture);
	}
}

void PuppetContactListener::playerPlayerContact(b2Contact * _contact){
	/*b2Fixture * fxA = _contact->GetFixtureA();
	b2Fixture * fxB = _contact->GetFixtureB();

	PuppetCharacter * puppetA = reinterpret_cast<PuppetCharacter *>( fxA->GetBody()->GetUserData() );
    PuppetCharacter * puppetB = reinterpret_cast<PuppetCharacter *>( fxB->GetBody()->GetUserData() );

	if(puppetA != nullptr && puppetB != nullptr){
		// Stuff
	}*/
}

void PuppetContactListener::playerItemContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _itemFixture){
	//std::cout << "Player-Item Collision" << std::endl;
	PuppetCharacter * p = static_cast<PuppetCharacter *>(_playerFixture->GetUserData());
	Item * item = static_cast<Item *>(_itemFixture->GetUserData());

	static_cast<Item *>(item)->hitPlayer();
	if(item->thrown || (item->held && item != p->heldItem)){
		// do some sort of damage thing here
		//PuppetResourceManager::hitSounds->playRandomSound();
        p->takeDamage();
	}else if(p->heldItem == nullptr && !item->held && !item->destroy){
		p->itemToPickup = item;
		// multiple players might be able to pick it up in one update
	}
}

void PuppetContactListener::playerStructureContact(b2Contact * contact, b2Fixture * playerFixture, b2Fixture * structureFixture){
	//std::cout << "Player-Structure Collision" << std::endl;

}

void PuppetContactListener::playerGroundContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture){
	//std::cout << "player-ground-contact" << std::endl;
	PuppetCharacter * puppet = static_cast<PuppetCharacter *>( _playerFixture->GetUserData());
	if(puppet != nullptr){
		puppet->canJump = true;
	}
}

void PuppetContactListener::structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture){
	static_cast<Item *>(_itemFixture->GetUserData())->hitStructure();
}

void PuppetContactListener::EndContact(b2Contact* _contact){
	b2Filter fA = _contact->GetFixtureA()->GetFilterData();
	b2Filter fB = _contact->GetFixtureB()->GetFilterData();

	b2Fixture * playerFixture = nullptr;
	if((fA.categoryBits & PuppetGame::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureA();
	}else if((fB.categoryBits & PuppetGame::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureB();
	}

	if(playerFixture != nullptr){
		PuppetCharacter * player = static_cast<PuppetCharacter *>(playerFixture->GetUserData());
		if(player != nullptr && player->deathPending){
			//player->die();
		}

		if((fB.categoryBits & PuppetGame::kGROUND) != 0 || (fA.categoryBits & PuppetGame::kGROUND) != 0){
			player->canJump = false;
		}
	}

	// behaviour stuff
	b2Fixture * behaviourFixture = nullptr;
	b2Fixture * otherFixture = nullptr;

	if((fA.categoryBits & PuppetGame::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureA();
		otherFixture = _contact->GetFixtureB();
	}else if((fB.categoryBits & PuppetGame::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureB();
		otherFixture = _contact->GetFixtureA();
	}

	if(behaviourFixture != nullptr){
		static_cast<Behaviour *>(behaviourFixture->GetUserData())->evaluateEndContact(otherFixture);
	}
}