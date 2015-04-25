#pragma once

#include "node/NodeAnimatable.h"
#include "Transform.h"
#include "Step.h"

#ifndef ANIMATION_H
#define ANIMATION_H
#include "Animation.h"
#endif

NodeAnimatable::NodeAnimatable() :
	NodeTransformable(nullptr)
{
	translateX	= new Animation<float>(&transform->translationVector.x);
	translateY	= new Animation<float>(&transform->translationVector.y);
	translateZ	= new Animation<float>(&transform->translationVector.z);
	rotate		= new Animation<glm::quat>(&transform->orientation);
	scaleX		= new Animation<float>(&transform->scaleVector.x);
	scaleY		= new Animation<float>(&transform->scaleVector.y);
	scaleZ		= new Animation<float>(&transform->scaleVector.z);
}

void NodeAnimatable::update(Step * _step){
	translateX->update(	_step);
	translateY->update(	_step);
	translateZ->update(	_step);
	rotate->update(		_step);
	scaleX->update(		_step);
	scaleY->update(		_step);
	scaleZ->update(		_step);
}

NodeAnimatable::~NodeAnimatable(){
	delete translateX;
	translateX = nullptr;
	delete translateY;
	translateY = nullptr;
	delete translateZ;
	translateZ = nullptr;
	delete rotate;
	rotate = nullptr;
	delete scaleX;
	scaleX = nullptr;
	delete scaleY;
	scaleY = nullptr;
	delete scaleZ;
	scaleZ = nullptr;
}