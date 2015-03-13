#pragma once

#include <PuppetCharacterCastleChampion.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <BehaviourManager.h>
#include <Item.h>

PuppetCharacterCastleChampion::PuppetCharacterCastleChampion(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		RaidTheCastleResourceManager::castleChampionTorso,
		RaidTheCastleResourceManager::castleChampionArm,
		RaidTheCastleResourceManager::castleChampionHelmet
	), true, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	behaviourManager.addBehaviour(new BehaviourPatrol(glm::vec3(50,0,0), glm::vec3(100,0,0), this, 10));
	behaviourManager.addBehaviour(new BehaviourAttack(this, 10, PuppetGame::kPLAYER));
	//behaviourManager.addBehaviour(new BehaviourFollow(this, 10, PuppetGame::kPLAYER));


	
	TextureSampler * weapon = RaidTheCastleResourceManager::itemFlailGrip;
	TextureSampler * weapon2 = RaidTheCastleResourceManager::itemFlailJoint;
	TextureSampler * weapon3 = RaidTheCastleResourceManager::itemFlailHead;
	itemToPickup = new Item(false, _world, PuppetGame::kITEM, PuppetGame::kPLAYER | PuppetGame::kSTRUCTURE | PuppetGame::kGROUND, _groupIndex, 0, 0, -weapon->height/2.f);

	componentScale = 0.008f;

	
	itemToPickup->rootComponent = new Box2DSprite(_world, b2_dynamicBody, false, nullptr, new Transform(), weapon->width, weapon->height, weapon->texture, componentScale/4);
	itemToPickup->components.push_back(&itemToPickup->rootComponent);
	
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}else{
		sf.maskBits = 0;
	}
	sf.groupIndex = groupIndex;
	
	for(Box2DSprite ** c : itemToPickup->components){
		(*c)->createFixture(sf);
	}

	itemToPickup->setUserData(this);

}

PuppetCharacterCastleChampion::~PuppetCharacterCastleChampion(){
	//delete texPack;
}

void PuppetCharacterCastleChampion::action(){
	// the castle champion isn't allowed to throw their weapon
}

void PuppetCharacterCastleChampion::update(Step * _step){
	PuppetCharacter::update(_step);

	if(behaviourManager.behaviours.at(1)->active){
		behaviourManager.behaviours.at(0)->active = false;
	}else{
		behaviourManager.behaviours.at(0)->active = true;
	}
}