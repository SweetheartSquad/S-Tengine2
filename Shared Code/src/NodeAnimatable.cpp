#pragma once

#include "NodeAnimatable.h"
#include "Transform.h"

NodeAnimatable::NodeAnimatable(Transform * _transform) : NodeTransformable(_transform),
	translateX(&transform->translationVector.x),translateY(&transform->translationVector.y),translateZ(&transform->translationVector.z),
	rotateX(&transform->orientation.x),rotateY(&transform->orientation.y),rotateZ(&transform->orientation.z),rotateW(&transform->orientation.w),
	scaleX(&transform->scaleVector.x),scaleY(&transform->scaleVector.y),scaleZ(&transform->scaleVector.z)
{
}

NodeAnimatable::~NodeAnimatable()
{
}