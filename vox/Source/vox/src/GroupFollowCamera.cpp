#pragma once

#include "Entity.h"
#include "GroupFollowCamera.h"
#include "System.h"
#include "Transform.h"

GroupFollowCamera::GroupFollowCamera(glm::vec3 _offset, float _deadZoneX, float _deadZoneY, float _deadZoneZ):
	PerspectiveCamera(),
	NodeTransformable(new Transform()),
	NodeAnimatable(),
	NodeUpdatable(),
	lastOrientation(1.f, 0.f, 0.f, 0.f),
	offset(_offset),
	deadZoneX(_deadZoneX),
	deadZoneY(_deadZoneY),
	deadZoneZ(_deadZoneZ)
{
}

GroupFollowCamera::~GroupFollowCamera(){
}

void GroupFollowCamera::update(Step * _step){

	lastOrientation = transform->orientation;

	transform->orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	transform->orientation = glm::rotate(transform->orientation, yaw, upVectorLocal);
	transform->orientation = glm::rotate(transform->orientation, pitch, rightVectorLocal);

	transform->orientation = glm::slerp(lastOrientation, transform->orientation, 0.15f * static_cast<float>(vox::deltaTimeCorrection));

	forwardVectorRotated   = transform->orientation * forwardVectorLocal;
	rightVectorRotated	   = transform->orientation * rightVectorLocal;
	upVectorRotated		   = transform->orientation * upVectorLocal;

	if(trans != nullptr){
		glm::vec3 oldLookAt = lookAtSpot;
		float xDif = (trans->transform->translationVector.x - transform->translationVector.x);
		if(xDif > deadZoneX){
			transform->translationVector.x += (xDif-deadZoneX);
			lookAtSpot.x += xDif-deadZoneX;
		}else if(xDif < -deadZoneX){
			transform->translationVector.x += (xDif+deadZoneX);
			lookAtSpot.x += xDif+deadZoneX;
		}
		//lookAtSpot.y = trans->transform->translationVector.y;
	}else{
		lookAtSpot = transform->translationVector+forwardVectorRotated;
	}
}

glm::mat4 GroupFollowCamera::getViewMatrix(){
	return glm::lookAt(
		transform->translationVector,	// Camera is here
		offset + lookAtSpot, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
		);
}

glm::mat4 GroupFollowCamera::getProjectionMatrix(){
	Dimension screenDimensions = vox::getScreenDimensions();
	// Projection matrix : 45° Field of View, ratio, near-far clip : 0.1 unit <-> 100 units
	return glm::perspective(fieldOfView, static_cast<float>(screenDimensions.width)/static_cast<float>(screenDimensions.height), nearClip, farClip);
}