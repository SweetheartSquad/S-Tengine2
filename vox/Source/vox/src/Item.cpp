#pragma once

#include "Item.h"
#include <Box2DWorld.h>

Item::Item(bool _singleUse, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex, int _damage, float _handleX, float _handleY):
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
	damage(_damage),
	handleX(_handleX),
	handleY(_handleY),
	singleUse(_singleUse),
	held(false),
	owner(nullptr),
	thrown(false),
	destroy(false),
	playerJoint(nullptr)
{
	componentScale *= 2.f;
}

Item::~Item(){
}

void Item::hitStructure(){
	if(!held){
		if(singleUse){
			destroy = true;
		}
	}
}

void Item::hitPlayer(){
	if(!held){
		if(singleUse){
			destroy = true;
		}
	}
}

void Item::hitGround(){
	if(!held){
		if(!singleUse){
			// set the item to be picked up again
			thrown = false;
			setGroupIndex(0);
			//std::cout << "item hit ground" << std::endl;
		}else{
			destroy = true;
			//std::cout << "item hit ground; should die" << std::endl;
		}
	}
}

Item * Item::getProjectile(){
	if(playerJoint != nullptr){
		world->b2world->DestroyJoint(playerJoint);
		playerJoint = nullptr;
		thrown = true;
		held = false;
	}
	return this;
}