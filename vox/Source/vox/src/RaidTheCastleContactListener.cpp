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

/*bool GameJamContactListener:: ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
	if(fixtureA->GetFilterData().maskBits == 1 || fixtureB->GetFilterData().maskBits == 1){
		// collide
	}else{
		// pass
	}
}*/

RaidTheCastleContactListener::RaidTheCastleContactListener(PuppetScene * _scene) :
	PuppetContactListener(_scene)
{
}

void RaidTheCastleContactListener::BeginContact(b2Contact * _contact){
	PuppetContactListener::BeginContact(_contact);
}

void RaidTheCastleContactListener::playerPlayerContact(b2Contact * _contact){
	PuppetContactListener::playerPlayerContact(_contact);
}

void RaidTheCastleContactListener::playerItemContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _itemFixture){
	PuppetContactListener::playerItemContact(_contact, _playerFixture, _itemFixture);
}

void RaidTheCastleContactListener::playerStructureContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _structureFixture){
	PuppetContactListener::playerStructureContact(_contact, _playerFixture, _structureFixture);
}

void RaidTheCastleContactListener::playerGroundContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture){
	PuppetContactListener::playerGroundContact(_contact, _playerFixture, _groundFixture);
}

void RaidTheCastleContactListener::structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture){
	PuppetContactListener::structureItemContact(_contact, _structureFixture, _itemFixture);

	Structure * structure = static_cast<Structure *>( _structureFixture->GetUserData() );
    Item * item = static_cast<Item *>( _itemFixture->GetUserData() );

	Castle * castle = dynamic_cast<Castle *>(structure);
	if(castle != nullptr){
		castle->takeDamage(item->damage); // what is going on here?
		Boulder * boulder = dynamic_cast<Boulder *>(item);
		if(boulder != nullptr){
			boulder->playerWhoFired->score += boulder->damage;
			if(castle->state == StructureBreakable::kDEAD){
				boulder->playerWhoFired->score += 100000;
			}
		}
	}
}

void RaidTheCastleContactListener::EndContact(b2Contact * _contact){
	PuppetContactListener::EndContact(_contact);
}