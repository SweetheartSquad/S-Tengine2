#pragma once

#include "Entity.h"
#include "PerspectiveCamera.h"
#include "Transform.h"

PerspectiveCamera::PerspectiveCamera() :
	lastOrientation(1.f, 0.f, 0.f, 0.f),
	interpolation(0.15f)
{
}

void PerspectiveCamera::update(Step * _step){
	lastOrientation = childTransform->getOrientationQuat();
	Camera::update(_step);
}

void PerspectiveCamera::setOrientation(glm::quat _orientation){
	if(interpolation == 0){
		_orientation = lastOrientation;
	}else if (interpolation == 1.f){
		_orientation = _orientation;
	}else{
		_orientation = glm::slerp(lastOrientation, _orientation, interpolation * static_cast<float>(sweet::deltaTimeCorrection));
	}
	Camera::setOrientation(_orientation);
}

glm::mat4 PerspectiveCamera::getProjectionMatrix(glm::vec2 _screenSize) const{
	return glm::perspective(fieldOfView, _screenSize.x/_screenSize.y, nearClip, farClip);
}