#pragma once

#include "NodeAnimatable.h"
#include "Transform.h"
#include "Step.h"

NodeAnimatable::NodeAnimatable(Transform * _transform) : NodeTransformable(_transform),
	translateX(&transform->translationVector.x),
	translateY(&transform->translationVector.y),
	translateZ(&transform->translationVector.z),
	rotateX(&transform->orientation.x),
	rotateY(&transform->orientation.y),
	rotateZ(&transform->orientation.z),
	rotateW(&transform->orientation.w),
	scaleX(&transform->scaleVector.x),
	scaleY(&transform->scaleVector.y),
	scaleZ(&transform->scaleVector.z)
{
}

void NodeAnimatable::update(Step * _step){
	translateX.update(	_step);
	translateY.update(	_step);
	translateZ.update(	_step);
	rotateX.update(		_step);
	rotateY.update(		_step);
	rotateZ.update(		_step);
	rotateW.update(		_step);
	scaleX.update(		_step);
	scaleY.update(		_step);
	scaleZ.update(		_step);
}