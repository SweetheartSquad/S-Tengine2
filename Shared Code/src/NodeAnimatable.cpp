#pragma once

#include "NodeAnimatable.h"
#include "Transform.h"
#include "Step.h"

NodeAnimatable::NodeAnimatable(Transform * _transform) :
	NodeTransformable(_transform)
{
	translateX	= new Animation(&transform->translationVector.x);
	translateY	= new Animation(&transform->translationVector.y);
	translateZ	= new Animation(&transform->translationVector.z);
	rotateX		= new Animation(&transform->orientation.x);
	rotateY		= new Animation(&transform->orientation.y);
	rotateZ		= new Animation(&transform->orientation.z);
	rotateW		= new Animation(&transform->orientation.w);
	scaleX		= new Animation(&transform->scaleVector.x);
	scaleY		= new Animation(&transform->scaleVector.y);
	scaleZ		= new Animation(&transform->scaleVector.z);
}

void NodeAnimatable::update(Step * _step){
	translateX->update(	_step);
	translateY->update(	_step);
	translateZ->update(	_step);
	rotateX->update(	_step);
	rotateY->update(	_step);
	rotateZ->update(	_step);
	rotateW->update(	_step);
	scaleX->update(		_step);
	scaleY->update(		_step);
	scaleZ->update(		_step);
}
NodeAnimatable::~NodeAnimatable(){
	delete translateX;
	delete translateY;
	delete translateZ;
	delete rotateX;
	delete rotateY;
	delete rotateZ;
	delete rotateW;
	delete scaleX;
	delete scaleY;
	delete scaleZ;
}