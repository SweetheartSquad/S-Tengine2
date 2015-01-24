#pragma once

#include "Sprite.h"
#include "PerspectiveCamera.h"
#include "System.h"
#include "Transform.h"

PerspectiveCamera::PerspectiveCamera(Sprite * _trans):
	Camera(),
	NodeTransformable(new Transform()),
	NodeAnimatable(),
	NodeUpdatable(),
	lastOrientation(1.f, 0.f, 0.f, 0.f),
	trans(_trans),
{
}

PerspectiveCamera::~PerspectiveCamera(){
}

void PerspectiveCamera::update(Step * _step){

	lastOrientation = transform->orientation;

	transform->orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	transform->orientation = glm::rotate(transform->orientation, yaw, upVectorLocal);
	transform->orientation = glm::rotate(transform->orientation, pitch, rightVectorLocal);

	transform->orientation = glm::slerp(lastOrientation, transform->orientation, 0.15f * static_cast<float>(vox::deltaTimeCorrection));

	forwardVectorRotated   = transform->orientation * forwardVectorLocal;
	rightVectorRotated	   = transform->orientation * rightVectorLocal;
	upVectorRotated		   = transform->orientation * upVectorLocal;
}

glm::mat4 PerspectiveCamera::getViewMatrix(){
	return glm::lookAt(
		transform->translationVector,	// Camera is here
		(trans == nullptr) ? (transform->translationVector+forwardVectorRotated) : trans->transform->translationVector, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
		);
}

glm::mat4 PerspectiveCamera::getProjectionMatrix(){
	Dimension screenDimensions = vox::getScreenDimensions();
	// Projection matrix : 45° Field of View, ratio, near-far clip : 0.1 unit <-> 100 units
	return glm::perspective(fieldOfView, static_cast<float>(screenDimensions.width)/static_cast<float>(screenDimensions.height), nearClip, farClip);
}