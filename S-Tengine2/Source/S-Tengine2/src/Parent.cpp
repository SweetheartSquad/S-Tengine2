#pragma once

#include <Parent.h>

Parent::Parent(Transform * _transform) :
	transform(_transform),
	cumulativeModelMatrixDirty(true),
	cumulativeModelMatrix(glm::mat4(1))
{
}

Parent::~Parent(){
	transform = nullptr;
}

void Parent::makeCumulativeModelMatrixDirty(){
	cumulativeModelMatrixDirty = true;
}