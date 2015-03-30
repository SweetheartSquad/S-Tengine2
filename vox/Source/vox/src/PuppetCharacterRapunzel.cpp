#pragma once

#include <PuppetCharacterRapunzel.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <RapunzelResourceManager.h>

#include <Item.h>

PuppetCharacterRapunzel::PuppetCharacterRapunzel(bool _ai, float _ghostPosition, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		RapunzelResourceManager::rapunzelTorso,
		RapunzelResourceManager::rapunzelArm,
		RapunzelResourceManager::rapunzelHelmet
	), _ghostPosition, _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{
	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != (int16)-1){
		sf.maskBits = maskBits;
	}else{
		sf.maskBits = 0;
	}
	sf.groupIndex = groupIndex;
}

PuppetCharacterRapunzel::~PuppetCharacterRapunzel(){
}