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
#include <StructureGoldPile.h>
#include <ItemGold.h>
#include <Rapunzel.h>
#include <vector>

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

	/*PuppetCharacter * puppet = static_cast<PuppetCharacter *>(_playerFixture->GetUserData());
	Structure * structure = static_cast<StructureGoldPile *>(_structureFixture->GetUserData());
	StructureGoldPile * goldPile = dynamic_cast<StructureGoldPile *>(structure);

	if(goldPile != nullptr) {
		if(!puppet->dead){
			goldPile->loseGold();
			if(goldPile->goldTaken < goldPile->totalGold){
				PuppetScene * pScene = static_cast<PuppetScene *>(scene);
				std::function<void(PuppetCharacter *)> fn = [pScene](PuppetCharacter * _puppet){
					ItemGold * gold = new ItemGold(pScene->world);
					gold->setShader(reinterpret_cast<Shader *>(pScene->shader), true);
					gold->rootComponent->setTranslationPhysical(_puppet->torso->body->GetPosition().x, _puppet->torso->body->GetPosition().y, 0);
					pScene->addChild(gold, 0);
					_puppet->action(true);
				};
				puppet->delegateToContactComplete(fn);
			}
		}
	}*/
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