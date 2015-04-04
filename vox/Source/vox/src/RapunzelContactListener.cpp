#include "RapunzelContactListener.h"
#include "PuppetScene.h"

#include "Scene.h"
#include "Hair.h"
#include "Lever.h"
#include "PuppetCharacter.h"
#include <PuppetCharacterRapunzel.h>
#include <PuppetCharacterGuard.h>
#include "Box2dWorld.h"
#include "Item.h"
#include "Box2DSprite.h"
#include "Box2DSuperSprite.h"
#include <Box2D/Box2D.h>
#include <Box2D\Dynamics\Joints\b2RevoluteJoint.h>

#include "Behaviour.h"

RapunzelContactListener::RapunzelContactListener(PuppetScene * _scene) :
	PuppetContactListener(_scene)
{
}

void RapunzelContactListener::BeginContact(b2Contact * _contact){
	PuppetContactListener::BeginContact(_contact);
}

void RapunzelContactListener::playerPlayerContact(b2Contact * _contact){
	PuppetContactListener::playerPlayerContact(_contact);
}

void RapunzelContactListener::playerItemContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _itemFixture){
	PuppetContactListener::playerItemContact(_contact, _playerFixture, _itemFixture);
}

void RapunzelContactListener::playerStructureContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _structureFixture){
	PuppetContactListener::playerStructureContact(_contact, _playerFixture, _structureFixture);
}

void RapunzelContactListener::playerGroundContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _groundFixture){
	PuppetContactListener::playerGroundContact(_contact, _playerFixture, _groundFixture);
}

void RapunzelContactListener::structureItemContact(b2Contact * _contact, b2Fixture * _structureFixture, b2Fixture * _itemFixture){
	PuppetContactListener::structureItemContact(_contact, _structureFixture, _itemFixture);
}

void RapunzelContactListener::EndContact(b2Contact * _contact){
	PuppetContactListener::EndContact(_contact);
}