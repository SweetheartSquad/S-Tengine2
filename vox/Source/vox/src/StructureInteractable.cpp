#pragma once

#include <StructureInteractable.h>

StructureInteractable::StructureInteractable(Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	ready(true),
	prepared(false),
	triggering(false),
	playerWhoTriggered(nullptr)
{
}

void StructureInteractable::trigger(PuppetCharacter * _playerWhoTriggered){
	playerWhoTriggered = _playerWhoTriggered;
	triggering = true;
	ready = false;
}

void StructureInteractable::update(Step * _step){
	Structure::update(_step);

	evaluateState();

	if(ready && !prepared){
		prepare();
	}

	if(triggering){
		attemptInteract();
	}
}


void StructureInteractable::prepare(){
	prepared = true;
}

void StructureInteractable::attemptInteract(){
	if(prepared){
		actuallyInteract();
	}
	prepared = false;
}
void StructureInteractable::actuallyInteract(){

}