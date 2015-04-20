#include "LD32_ContactListener.h"
#include "LD32_Scene.h"

#include "Scene.h"
#include "LD32_Character.h"
#include "Box2dWorld.h"
#include "Item.h"
#include "Box2DSprite.h"
#include "Box2DSuperSprite.h"
#include <Box2D/Box2D.h>
#include <SoundManager.h>
#include <StructureInteractable.h>

#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

#include "Behaviour.h"

/*bool GameJamContactListener:: ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
if(fixtureA->GetFilterData().maskBits == 1 || fixtureB->GetFilterData().maskBits == 1){
// collide
}else{
// pass
}
}*/

LD32_ContactListener::LD32_ContactListener(LD32_Scene * _scene) :
scene(_scene),
damageScoreMult(0.f)
{
	_scene->world->b2world->SetContactListener(this);
}

void LD32_ContactListener::BeginContact(b2Contact * _contact){
	b2Filter fA = _contact->GetFixtureA()->GetFilterData();
	b2Filter fB = _contact->GetFixtureB()->GetFilterData();
	b2Fixture * playerFixture = nullptr;
	b2Fixture * otherFixture = nullptr;
	if ((fA.categoryBits & LD32_Game::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureA();
		otherFixture = _contact->GetFixtureB();
	}
	else if ((fB.categoryBits & LD32_Game::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureB();
		otherFixture = _contact->GetFixtureA();
	}
	//std::cout << fA.categoryBits << " | " << fB.categoryBits << std::endl;

	if (playerFixture != nullptr){
		if ((fA.categoryBits & LD32_Game::kPLAYER) != 0 && (fB.categoryBits & LD32_Game::kPLAYER) != 0){
			// Player-Player collision
			playerPlayerContact(_contact);
		}
		else if ((fA.categoryBits & LD32_Game::kITEM) != 0 || (fB.categoryBits & LD32_Game::kITEM) != 0){
			// Player-Item collision
			//std::cout << fA.categoryBits << "|" << fB.categoryBits << std::endl;
			playerItemContact(_contact, playerFixture, otherFixture);
		}
		else if ((fA.categoryBits & LD32_Game::kSTRUCTURE) != 0 || (fB.categoryBits & LD32_Game::kSTRUCTURE) != 0){
			// Player-Structure collision
			playerStructureContact(_contact, playerFixture, otherFixture);
		}
		else if ((fA.categoryBits & LD32_Game::kGROUND) != 0 || (fB.categoryBits & LD32_Game::kGROUND) != 0){
			// Player-Ground collision
			playerGroundContact(_contact, playerFixture, otherFixture);
		}
		else if ((fA.categoryBits & LD32_Game::kDEAD_ZONE) != 0 || (fB.categoryBits & LD32_Game::kDEAD_ZONE) != 0){
			// Player-Dead Zone collision
			playerDeadZoneContact(_contact, playerFixture, otherFixture);
		}
	}
	else{
		// neither of the fixtures is a player

		// if a thrown item hits the ground, make it not thrown and able to collide regularly
		if ((fA.categoryBits & LD32_Game::kGROUND) != 0 && (fB.categoryBits & LD32_Game::kITEM) != 0){
			Item * item = static_cast<Item *>(_contact->GetFixtureB()->GetUserData());
			item->hitGround();
		}
		else if ((fB.categoryBits & LD32_Game::kGROUND) != 0 && (fA.categoryBits & LD32_Game::kITEM) != 0){
			Item * item = static_cast<Item *>(_contact->GetFixtureA()->GetUserData());
			item->hitGround();
		}

		// structure - item?
		b2Fixture * structureFixture = nullptr;
		b2Fixture * itemFixture = nullptr;
		bool structure = false;
		if ((fA.categoryBits & LD32_Game::kSTRUCTURE) != 0 && (fB.categoryBits & LD32_Game::kITEM) != 0){
			structureFixture = _contact->GetFixtureA();
			itemFixture = _contact->GetFixtureB();
			structure = true;
		}
		else if ((fB.categoryBits & LD32_Game::kSTRUCTURE) != 0 && (fA.categoryBits & LD32_Game::kITEM) != 0){
			structureFixture = _contact->GetFixtureB();
			itemFixture = _contact->GetFixtureA();
			structure = true;
		}

		if (structure){
			structureItemContact(_contact, structureFixture, itemFixture);
		}
	}

	// behaviour stuff
	b2Fixture * behaviourFixture = nullptr;

	if ((fA.categoryBits & LD32_Game::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureA();
		otherFixture = _contact->GetFixtureB();
	}
	else if ((fB.categoryBits & LD32_Game::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureB();
		otherFixture = _contact->GetFixtureA();
	}

	if (behaviourFixture != nullptr){
		static_cast<Behaviour *>(behaviourFixture->GetUserData())->evaluateBeginContact(otherFixture);
	}
}

void LD32_ContactListener::playerPlayerContact(b2Contact * _contact){
	b2Fixture * fxA = _contact->GetFixtureA();
	b2Fixture * fxB = _contact->GetFixtureB();

	if (fxA->GetUserData() != nullptr && fxB->GetUserData() != nullptr){
		LD32_Character * LD32_A = static_cast<LD32_Character *>(fxA->GetUserData());
		LD32_Character * LD32_B = static_cast<LD32_Character *>(fxB->GetUserData());
		/*if(LD32_A->control < 0.5f && LD32_B->control > 0.5f){
		LD32_B->canJump = true;
		}else if(LD32_B->control < 0.5f && LD32_A->control > 0.5f){
		LD32_A->canJump = true;
		}*/
		LD32_A->canJump = true;
		LD32_B->canJump = true;
		LD32_A->addCollision(LD32_Game::kPLAYER);
		LD32_B->addCollision(LD32_Game::kPLAYER);
	}
}

void LD32_ContactListener::playerItemContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _itemFixture){
	//std::cout << "Player-Item Collision" << std::endl;
	LD32_Character * p = static_cast<LD32_Character *>(_playerFixture->GetUserData());
	Item * item = static_cast<Item *>(_itemFixture->GetUserData());

	// if an item is triggered as dead, don't trigger a proper contact
	if (!item->destroy){
		p->addCollision(LD32_Game::kITEM);
		static_cast<Item *>(item)->hitPlayer();
		if (item->thrown || (item->held && item != p->heldItem)){
			// do some sort of damage thing here
			//LD32_ResourceManager::hitSounds->playRandomSound();
			p->takeDamage(item->damage);
			item->owner->score += item->damage * damageScoreMult;
		}
		else if (p->heldItem == nullptr && !item->held && !item->destroy){
			p->itemToPickup = item;
			// multiple players might be able to pick it up in one update
		}
	}
}

void LD32_ContactListener::playerStructureContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _structureFixture){
	//std::cout << "Player-Structure Collision" << std::endl;
	LD32_Character * p = static_cast<LD32_Character *>(_playerFixture->GetUserData());
	if (p != nullptr) {
		p->addCollision(LD32_Game::kSTRUCTURE);

		StructureInteractable * si = dynamic_cast<StructureInteractable *>(static_cast<Structure *>(_structureFixture->GetUserData()));
		if (si != nullptr){
			si->trigger(p);
		}
	}
}

void LD32_ContactListener::playerGroundContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture){
	//std::cout << "player-ground-contact" << std::endl;
	LD32_Character * LD32_ = static_cast<LD32_Character *>(_playerFixture->GetUserData());
	if (LD32_ != nullptr){
		LD32_->addCollision(LD32_Game::kGROUND);
		LD32_->canJump = true;
	}
}

void LD32_ContactListener::playerDeadZoneContact(b2Contact* b2_contact, b2Fixture* player_fixture, b2Fixture* other_fixture){
	LD32_Character * player = static_cast<LD32_Character*>(player_fixture->GetUserData());
	if (player != nullptr) {
		player->deathPending = true;
		LD32_ResourceManager::fallingSounds->playRandomSound();
	}
}

void LD32_ContactListener::structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture){
	static_cast<Item *>(_itemFixture->GetUserData())->hitStructure();
}

void LD32_ContactListener::EndContact(b2Contact* _contact){
	b2Filter fA = _contact->GetFixtureA()->GetFilterData();
	b2Filter fB = _contact->GetFixtureB()->GetFilterData();

	b2Fixture * playerFixture = nullptr;
	LD32_Character * player = nullptr;
	if ((fA.categoryBits & LD32_Game::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureA();
		//We pretty much know its a LD32_ character because of the category bits
		player = static_cast<LD32_Character *>(playerFixture->GetUserData());
		if (player != nullptr){
			//	NodeTransformable * s = reinterpret_cast<NodeTransformable *>(player);
			if ((fB.categoryBits & LD32_Game::kPLAYER) != 0) {
				player->removeCollision(LD32_Game::kPLAYER);
			}
			if ((fB.categoryBits & LD32_Game::kGROUND) != 0) {
				player->removeCollision(LD32_Game::kGROUND);
			}
			if ((fB.categoryBits & LD32_Game::kSTRUCTURE) != 0) {
				player->removeCollision(LD32_Game::kSTRUCTURE);
			}
			if ((fB.categoryBits & LD32_Game::kITEM) != 0) {
				player->removeCollision(LD32_Game::kITEM);
			}
			if (!player->isCollidingWith(LD32_Game::kGROUND) && !player->isCollidingWith(LD32_Game::kPLAYER)){
				player->canJump = false;
			}
		}
	}
	if ((fB.categoryBits & LD32_Game::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureB();
		player = static_cast<LD32_Character *>(playerFixture->GetUserData());
		if (player != nullptr) {
			if ((fA.categoryBits & LD32_Game::kPLAYER) != 0) {
				player->removeCollision(LD32_Game::kPLAYER);
			}
			if ((fA.categoryBits & LD32_Game::kGROUND) != 0) {
				player->removeCollision(LD32_Game::kGROUND);
			}
			if ((fA.categoryBits & LD32_Game::kSTRUCTURE) != 0) {
				player->removeCollision(LD32_Game::kSTRUCTURE);
			}
			if ((fA.categoryBits & LD32_Game::kITEM) != 0) {
				player->removeCollision(LD32_Game::kITEM);
			}
			if (!player->isCollidingWith(LD32_Game::kGROUND)){
				player->canJump = false;
			}
		}
	}

	if (playerFixture != nullptr){
		LD32_Character * player = static_cast<LD32_Character *>(playerFixture->GetUserData());
		if (player != nullptr && player->deathPending){
			//player->die();
		}
		//&& !player->isCollidingWith(LD32_Game::kPLAYER)
	}

	// behaviour stuff
	b2Fixture * behaviourFixture = nullptr;
	b2Fixture * otherFixture = nullptr;

	if ((fA.categoryBits & LD32_Game::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureA();
		otherFixture = _contact->GetFixtureB();
	}
	else if ((fB.categoryBits & LD32_Game::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureB();
		otherFixture = _contact->GetFixtureA();
	}

	if (behaviourFixture != nullptr){
		Behaviour * b = static_cast<Behaviour *>(behaviourFixture->GetUserData());
		b->evaluateEndContact(otherFixture);
	}
}