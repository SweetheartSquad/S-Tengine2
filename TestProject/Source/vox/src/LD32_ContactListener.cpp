#include <LD32_ContactListener.h>
#include <LD32_Scene.h>
#include <LD32_Game.h>
#include <LD32_Enemy.h>
#include <LD32_ResourceManager.h>
#include <LD32_Player.h>
#include <LD32_Donut.h>

#include "Scene.h"
#include "Box2dWorld.h"
#include "Box2DSprite.h"
#include "Box2DSuperSprite.h"
#include <Box2D/Box2D.h>
#include <SoundManager.h>

#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

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
		if ((fA.categoryBits & LD32_Game::kENEMY) != 0 || (fB.categoryBits & LD32_Game::kENEMY) != 0){
			playerEnemyContact(_contact, playerFixture, otherFixture);
		} if ((fA.categoryBits & LD32_Game::kBUMPER) != 0 || (fB.categoryBits & LD32_Game::kBUMPER) != 0){
			playerBumperContact(_contact, playerFixture, otherFixture);
		} if ((fA.categoryBits & LD32_Game::kBOUNDARY) != 0 || (fB.categoryBits & LD32_Game::kBOUNDARY) != 0){
			playerBoundaryContact(_contact, playerFixture, otherFixture);
		} if ((fA.categoryBits & LD32_Game::kDEAD_ZONE) != 0 || (fB.categoryBits & LD32_Game::kDEAD_ZONE) != 0){
			playerDeadZoneContact(_contact, playerFixture, otherFixture);
		} if ((fA.categoryBits & LD32_Game::kMONSTER) != 0 || (fB.categoryBits & LD32_Game::kMONSTER) != 0){
			playerMonsterContact(_contact, playerFixture, otherFixture);
		}
	}else{
		
	}
}


void LD32_ContactListener::EndContact(b2Contact* _contact){
	b2Filter fA = _contact->GetFixtureA()->GetFilterData();
	b2Filter fB = _contact->GetFixtureB()->GetFilterData();

	b2Fixture * playerFixture = nullptr;
	LD32_Player * player = nullptr;
	if ((fA.categoryBits & LD32_Game::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureA();
		//We pretty much know its a LD32_ character because of the category bits
		player = static_cast<LD32_Player *>(playerFixture->GetUserData());
		if (player != nullptr){
			//	NodeTransformable * s = reinterpret_cast<NodeTransformable *>(player);
			/*if ((fB.categoryBits & LD32_Game::kPLAYER) != 0) {
				player->removeCollision(LD32_Game::kPLAYER);
			}
			if ((fB.categoryBits & LD32_Game::kBOUNDARY) != 0) {
				player->removeCollision(LD32_Game::kBOUNDARY);
			}
			if ((fB.categoryBits & LD32_Game::kENEMY) != 0) {
				player->removeCollision(LD32_Game::kENEMY);
			}
			if ((fB.categoryBits & LD32_Game::kBUMPER) != 0) {
				player->removeCollision(LD32_Game::kBUMPER);
			}*/
		}
	}
	if ((fB.categoryBits & LD32_Game::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureB();
		player = static_cast<LD32_Player *>(playerFixture->GetUserData());
		/*if (player != nullptr) {
			if ((fA.categoryBits & LD32_Game::kPLAYER) != 0) {
				player->removeCollision(LD32_Game::kPLAYER);
			}
			if ((fA.categoryBits & LD32_Game::kBOUNDARY) != 0) {
				player->removeCollision(LD32_Game::kBOUNDARY);
			}
			if ((fA.categoryBits & LD32_Game::kENEMY) != 0) {
				player->removeCollision(LD32_Game::kENEMY);
			}
			if ((fA.categoryBits & LD32_Game::kBUMPER) != 0) {
				player->removeCollision(LD32_Game::kBUMPER);
			}
		}*/
	}

	if (playerFixture != nullptr){
		LD32_Player * player = static_cast<LD32_Player *>(playerFixture->GetUserData());
		if (player != nullptr && player->deathPending){
			//player->die();
		}
		//&& !player->isCollidingWith(LD32_Game::kPLAYER)
	}

}

void LD32_ContactListener::playerBumperContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _bumperFixture){
	LD32_Player * player = static_cast<LD32_Player*>(_playerFixture->GetUserData());
	LD32_Donut * donut = static_cast<LD32_Donut*>(_bumperFixture->GetUserData());
	
	LD32_ResourceManager::bumperSfx->playRandomSound();
	donut->hit();
}
void LD32_ContactListener::playerBoundaryContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _boundaryFixture){
	/*LD32_Player * player = static_cast<LD32_Player*>(player_fixture->GetUserData());
	if (player != nullptr) {
		LD32_ResourceManager::miscSounds->play("bumper");
	}*/
}
void LD32_ContactListener::playerEnemyContact(b2Contact* b2_contact, b2Fixture * _playerFixture, b2Fixture * _enemyFixture){
	LD32_Player * player = static_cast<LD32_Player*>(_playerFixture->GetUserData());
	LD32_Enemy * enemy = static_cast<LD32_Enemy*>(_enemyFixture->GetUserData());
	if(enemy != nullptr){
		if(!enemy->heDed){
			if (player != nullptr) {
				++player->hits;
				if(player->hits > 3){
					player->deathPending = true;
					LD32_ResourceManager::miscSounds->play("death");
				}else{
					LD32_ResourceManager::enemySfx->playRandomSound();
				}
			}
			enemy->heDed = true;
		}
	}
}
void LD32_ContactListener::playerDeadZoneContact(b2Contact* b2_contact, b2Fixture* _playerFixture, b2Fixture* _deadZoneFixture){
	LD32_Player * player = static_cast<LD32_Player*>(_playerFixture->GetUserData());
	if (player != nullptr) {
		player->deathPending = true;
		LD32_ResourceManager::miscSounds->play("death");
	}
}
void LD32_ContactListener::playerMonsterContact(b2Contact* b2_contact, b2Fixture* _playerFixture, b2Fixture* _monsterFixture){
	LD32_Player * player = static_cast<LD32_Player*>(_playerFixture->GetUserData());
	if (player != nullptr) {
		player->won = true;
		LD32_ResourceManager::miscSounds->play("win");
	}
}