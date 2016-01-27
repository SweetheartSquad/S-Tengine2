#pragma once

#include "Entity.h"
#include "PerspectiveCamera.h"
#include "Transform.h"

PerspectiveCamera::PerspectiveCamera() :
	lastOrientation(1.f, 0.f, 0.f, 0.f),
	interpolation(0.15f)
{
}

PerspectiveCamera::~PerspectiveCamera(){
}

void PerspectiveCamera::update(Step * _step){
	lastOrientation = childTransform->getOrientationQuat();

	glm::quat newOrientation = calcOrientation();
	newOrientation = glm::slerp(lastOrientation, newOrientation, interpolation * static_cast<float>(sweet::deltaTimeCorrection));

	childTransform->setOrientation(newOrientation);

	forwardVectorRotated   = newOrientation * forwardVectorLocal;
	rightVectorRotated	   = newOrientation * rightVectorLocal;
	upVectorRotated		   = newOrientation * upVectorLocal;

	lookAtSpot = childTransform->getWorldPos()+forwardVectorRotated;
	Camera::update(_step);
}

glm::mat4 PerspectiveCamera::getViewMatrix() const{
	return glm::lookAt(
		childTransform->getWorldPos(),	// Camera is here
		lookAtSpot + lookAtOffset,			// and looks here : at the same position, plus "direction"
		upVectorRotated						// Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 PerspectiveCamera::getProjectionMatrix() const{
	glm::vec2 screenDimensions = sweet::getWindowDimensions();
	return glm::perspective(fieldOfView, static_cast<float>(screenDimensions.x)/static_cast<float>(screenDimensions.y), nearClip, farClip);
}