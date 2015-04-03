#pragma once

#include "Entity.h"
#include "PerspectiveCamera.h"
#include "System.h"
#include "Transform.h"

PerspectiveCamera::PerspectiveCamera():
	Camera(),
	NodeTransformable(new Transform()),
	//NodeAnimatable(),
	NodeUpdatable(),
	lastOrientation(1.f, 0.f, 0.f, 0.f)
{
}

PerspectiveCamera::~PerspectiveCamera(){
}

void PerspectiveCamera::update(Step * _step){
	lastOrientation = transform->getOrientationQuat();
	glm::quat newOrientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	newOrientation = glm::rotate(newOrientation, yaw, upVectorLocal);
	newOrientation = glm::rotate(newOrientation, pitch, rightVectorLocal);
	
	glm::rotate(transform->getOrientationQuat(), pitch, rightVectorLocal);

	newOrientation = glm::slerp(lastOrientation, newOrientation, 0.15f * static_cast<float>(vox::deltaTimeCorrection));
	transform->setOrientation(newOrientation);

	forwardVectorRotated   = newOrientation * forwardVectorLocal;
	rightVectorRotated	   = newOrientation * rightVectorLocal;
	upVectorRotated		   = newOrientation * upVectorLocal;

	lookAtSpot = transform->getTranslationVector()+forwardVectorRotated;
}

glm::mat4 PerspectiveCamera::getViewMatrix(){
	return glm::lookAt(
		transform->getTranslationVector(),	// Camera is here
		lookAtSpot,						// and looks here : at the same position, plus "direction"
		upVectorRotated					// Head is up (set to 0,-1,0 to look upside-down)
		);
}

glm::mat4 PerspectiveCamera::getProjectionMatrix(){
	Dimension screenDimensions = vox::getScreenDimensions();
	// Projection matrix : 45° Field of View, ratio, near-far clip : 0.1 unit <-> 100 units
	return glm::perspective(fieldOfView, static_cast<float>(screenDimensions.width)/static_cast<float>(screenDimensions.height), nearClip, farClip);
}