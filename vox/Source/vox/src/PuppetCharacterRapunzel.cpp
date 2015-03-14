#pragma once

#include <PuppetCharacterRapunzel.h>
#include <PuppetTexturePack.h>
#include <BehaviourPatrol.h>
#include <BehaviourAttack.h>
#include <RapunzelResourceManager.h>

#include <Item.h>

PuppetCharacterRapunzel::PuppetCharacterRapunzel(bool _ai, unsigned long int _id, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	PuppetCharacter(new PuppetTexturePack(
		(_id == 0) ? RapunzelResourceManager::rapunzelRedTorso : (_id == 1 ? RapunzelResourceManager::rapunzelGreenTorso : (_id == 2 ? RapunzelResourceManager::rapunzelBlueTorso : RapunzelResourceManager::rapunzelYellowTorso)),
		(_id == 0) ? RapunzelResourceManager::rapunzelRedArm : (_id == 1 ? RapunzelResourceManager::rapunzelGreenArm : (_id == 2 ? RapunzelResourceManager::rapunzelBlueArm : RapunzelResourceManager::rapunzelYellowArm)),
		(_id == 0) ? RapunzelResourceManager::rapunzelRedHelmet : (_id == 1 ? RapunzelResourceManager::rapunzelGreenHelmet : (_id == 2 ? RapunzelResourceManager::rapunzelBlueHelmet : RapunzelResourceManager::rapunzelYellowHelmet))
	), _ai, _world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr)
{

	componentScale = 0.008f;

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