#pragma once

#include "SlayTheDragonContactListener.h"
#include "PuppetScene.h"

#include "Scene.h"
#include "Hair.h"
#include "Lever.h"
#include "PuppetCharacterArcher.h"
#include <PuppetCharacterDragon.h>
#include "Box2dWorld.h"
#include "Item.h"
#include "Box2DSprite.h"
#include "Box2DSuperSprite.h"
#include <Box2D/Box2D.h>
#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

#include <SoundManager.h>
#include <SlayTheDragonResourceManager.h>
#include "Behaviour.h"
#include <Fortification.h>
#include <ItemFireball.h>

SlayTheDragonContactListener::SlayTheDragonContactListener(PuppetScene * _scene) :
	PuppetContactListener(_scene)
{
}

void SlayTheDragonContactListener::BeginContact(b2Contact * _contact){
	PuppetContactListener::BeginContact(_contact);
}

void SlayTheDragonContactListener::playerPlayerContact(b2Contact * _contact){
	PuppetContactListener::playerPlayerContact(_contact);
}

void SlayTheDragonContactListener::playerItemContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _itemFixture){
	PuppetContactListener::playerItemContact(_contact, _playerFixture, _itemFixture);
	
	PuppetCharacterArcher * player = static_cast<PuppetCharacterArcher *>( _playerFixture->GetUserData() );
    Item * item = static_cast<Item *>( _itemFixture->GetUserData() );

	ItemFireball * fb = dynamic_cast<ItemFireball *>(item);
	if(fb != nullptr){
		static_cast<PuppetCharacterDragon *>(fb->owner)->playerOnFire = player;
		player->onFire = true;
	}
}

void SlayTheDragonContactListener::playerStructureContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _structureFixture){
	PuppetContactListener::playerStructureContact(_contact, _playerFixture, _structureFixture);
}

void SlayTheDragonContactListener::playerGroundContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture){
	PuppetContactListener::playerGroundContact(_contact, _playerFixture, _groundFixture);
}

void SlayTheDragonContactListener::playerDeadZoneContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture){
	PuppetContactListener::playerDeadZoneContact(_contact, _playerFixture, _groundFixture);
}

void SlayTheDragonContactListener::structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture){
	PuppetContactListener::structureItemContact(_contact, _structureFixture, _itemFixture);

	Structure * structure = static_cast<Structure *>( _structureFixture->GetUserData() );
    Item * item = static_cast<Item *>( _itemFixture->GetUserData() );

	Fortification * fort = dynamic_cast<Fortification *>(structure);
	if(fort != nullptr){
		// need to filter by roof vs. base (base shouldn't take damage)

		fort->takeDamage(item->damage); // what is going on here?
		/*ItemFireball * fireball = dynamic_cast<ItemFireball *>(item);
		if(fireball != nullptr){
			//fireball->playerWhoFired->score += boulder->damage;
			if(fort->state == StructureBreakable::kDEAD){
				fireball->playerWhoFired->score += 100000;
			}
		}*/
	}
}

void SlayTheDragonContactListener::EndContact(b2Contact * _contact){
	PuppetContactListener::EndContact(_contact);
}