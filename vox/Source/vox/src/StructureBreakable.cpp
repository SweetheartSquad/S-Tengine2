#pragma once

#include <StructureBreakable.h>

StructureBreakable::StructureBreakable(float _maxHealth, Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex):
	Structure(_world, _categoryBits, _maskBits, _groupIndex),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	health(_maxHealth),
	maxHealth(_maxHealth),
	state(kNORMAL)
{
}

void StructureBreakable::takeDamage(float _damage){
	health -= _damage;
	if(health <= 0.f){
		state = kDEAD;
		health = 0;
	}else if(health <= maxHealth * 0.5f){
		state = kDAMAGED;
	}else{
		state = kNORMAL;
		if(health >= maxHealth){
			health = maxHealth;
		}
	}
}