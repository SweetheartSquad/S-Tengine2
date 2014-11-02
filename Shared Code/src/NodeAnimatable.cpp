#pragma once

#include "NodeAnimatable.h"

NodeAnimatable::NodeAnimatable(Transform * _transform): 
	translateX(&_transform->translationVector.x),translateY(&_transform->translationVector.y),translateZ(&_transform->translationVector.z),
	rotateX(&_transform->orientation.x),rotateY(&_transform->orientation.y),rotateZ(&_transform->orientation.z),rotateW(&_transform->orientation.w),
	scaleX(&_transform->scaleVector.x),scaleY(&_transform->scaleVector.y),scaleZ(&_transform->scaleVector.z)
{
}

NodeAnimatable::~NodeAnimatable()
{
}