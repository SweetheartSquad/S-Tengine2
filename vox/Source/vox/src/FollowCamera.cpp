#pragma once

#include "Entity.h"
#include "FollowCamera.h"
#include "System.h"
#include "Transform.h"

FollowCamera::FollowCamera(glm::vec3 _offset, float _deadZoneX, float _deadZoneY, float _deadZoneZ):
	PerspectiveCamera(),
	NodeTransformable(new Transform()),
	NodeAnimatable(),
	NodeUpdatable(),
	lastOrientation(1.f, 0.f, 0.f, 0.f),
	offset(_offset),
	deadZoneX(_deadZoneX),
	deadZoneY(_deadZoneY),
	deadZoneZ(_deadZoneZ),
	minimumZoom(10)
{
}

FollowCamera::~FollowCamera(){
}

void FollowCamera::update(Step * _step){

	lastOrientation = transform->orientation;

	transform->orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	transform->orientation = glm::rotate(transform->orientation, yaw, upVectorLocal);
	transform->orientation = glm::rotate(transform->orientation, pitch, rightVectorLocal);

	transform->orientation = glm::slerp(lastOrientation, transform->orientation, 0.15f * static_cast<float>(vox::deltaTimeCorrection));

	forwardVectorRotated   = transform->orientation * forwardVectorLocal;
	rightVectorRotated	   = transform->orientation * rightVectorLocal;
	upVectorRotated		   = transform->orientation * upVectorLocal;
	
	lookAtSpot = glm::vec3(0,0,0);
	float minX = 9999999999;
	float minY = 9999999999;
	float maxX = -9999999999;
	float maxY = -9999999999;
	for(ShiftKiddie * nt : targets){
		glm::vec3 pos = nt->getPos(false);
		//lookAtSpot += pos;
		minX = std::min(pos.x, minX);
		maxX = std::max(pos.x, maxX);
		minY = std::min(pos.y, minY);
		maxY = std::max(pos.y, maxY);
	}
	float screenWidth = maxX - minX;
	float screenHeight = maxY - minY;
	float zoom = std::max(minimumZoom, std::max(screenWidth, screenHeight));

	/*if(targets.size() > 1){
		lookAtSpot /= targets.size();
	}*/
	std::cout << screenWidth << "\t" << screenHeight << std::endl;
	glm::vec3 oldLookAt = lookAtSpot;


	// move camera
	lookAtSpot.x = transform->translationVector.x = minX + screenWidth/2.f;
	lookAtSpot.y = transform->translationVector.y = minY + screenHeight/2.f;
	transform->translationVector.z = zoom;

	/*float xDif = (lookAtSpot.x - transform->translationVector.x);
	if(xDif > deadZoneX){
		transform->translationVector.x += (xDif-deadZoneX);
		lookAtSpot.x += xDif-deadZoneX;
	}else if(xDif < -deadZoneX){
		transform->translationVector.x += (xDif+deadZoneX);
		lookAtSpot.x += xDif+deadZoneX;
	}*/
}

glm::mat4 FollowCamera::getViewMatrix(){
	return glm::lookAt(
		transform->translationVector,	// Camera is here
		offset + lookAtSpot, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
		);
}

glm::mat4 FollowCamera::getProjectionMatrix(){
	Dimension screenDimensions = vox::getScreenDimensions();
	// Projection matrix : 45° Field of View, ratio, near-far clip : 0.1 unit <-> 100 units
	return glm::perspective(fieldOfView, static_cast<float>(screenDimensions.width)/static_cast<float>(screenDimensions.height), nearClip, farClip);
}

void FollowCamera::addTarget(ShiftKiddie * _target){
	targets.push_back(_target);
}