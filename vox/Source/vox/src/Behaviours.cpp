#pragma once

#include "Behaviours.h"

#include "PuppetCharacter.h"
#include "Box2DSprite.h"

void Behaviours::followX(PuppetCharacter * b){
	b->torso->applyLinearImpulseUp(50);
}