#pragma once

#include "Entity.h"
#include "PerspectiveCamera.h"
#include "System.h"
#include "Transform.h"

PerspectiveCamera::PerspectiveCamera() :
	lastOrientation(1.f, 0.f, 0.f, 0.f),
	interpolation(0.15f)
{
}

PerspectiveCamera::~PerspectiveCamera(){
}

void PerspectiveCamera::update(Step * _step){
	lastOrientation = parent->getOrientationQuat();

	glm::quat newOrientation = calcOrientation();
	newOrientation = glm::slerp(lastOrientation, newOrientation, interpolation * static_cast<float>(vox::deltaTimeCorrection));

	parent->setOrientation(newOrientation);

	forwardVectorRotated   = newOrientation * forwardVectorLocal;
	rightVectorRotated	   = newOrientation * rightVectorLocal;
	upVectorRotated		   = newOrientation * upVectorLocal;

	lookAtSpot = parent->getTranslationVector()+forwardVectorRotated;
}

glm::mat4 PerspectiveCamera::getViewMatrix(){
	return glm::lookAt(
		parent->getTranslationVector(),	// Camera is here
		lookAtSpot + lookAtOffset,			// and looks here : at the same position, plus "direction"
		upVectorRotated						// Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 PerspectiveCamera::getProjectionMatrix(){
	glm::vec2 screenDimensions = vox::getScreenDimensions();
	// Projection matrix : 45° Field of View, ratio, near-far clip : 0.1 unit <-> 100 units
	return glm::perspective(fieldOfView, static_cast<float>(screenDimensions.x)/static_cast<float>(screenDimensions.y), nearClip, farClip);
}