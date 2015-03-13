#pragma once

#include "Entity.h"
#include "FollowCamera.h"
#include "System.h"
#include "Transform.h"

FollowCamera::FollowCamera(float _buffer, glm::vec3 _offset, float _deadZoneX, float _deadZoneY, float _deadZoneZ):
	PerspectiveCamera(),
	NodeTransformable(new Transform()),
	NodeAnimatable(),
	NodeUpdatable(),
	buffer(_buffer),
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
	
	lookAtSpot = glm::vec3(0.f, 0.f, 0.f);
	float minX = 9999999999.f;
	float minY = 9999999999.f;
	float maxX = -9999999999.f;
	float maxY = -9999999999.f;
	for(ShiftKiddie * nt : targets){
		glm::vec3 pos = nt->getPos(false);
		//lookAtSpot += pos;
		minX = std::min(pos.x-buffer, minX);
		maxX = std::max(pos.x+buffer, maxX);
		minY = std::min(pos.y-buffer, minY);
		maxY = std::max(pos.y+buffer, maxY);
	}
	float screenWidth = maxX - minX;
	float screenHeight = maxY - minY;

	// account for FoV (the camera FoV seems to be vertical, so if the screen w > screen h, we need to take the h / the intended aspect ratio)
	float ar1 = screenWidth/screenHeight;
	Dimension screenDimensions = vox::getScreenDimensions();
	float ar2 = static_cast<float>(screenDimensions.width)/static_cast<float>(screenDimensions.height);
	float zoom;
	if(ar1 > ar2){
		zoom = std::max(minimumZoom, screenWidth / ar2);
	}else if(ar1 < ar2){
		zoom = std::max(minimumZoom, screenHeight);
	}else{
		zoom = std::max(minimumZoom, screenHeight);
	}

	float dist = zoom / (tan(glm::radians(fieldOfView) * 0.5f) * 2.f);
	
	glm::vec3 oldLookAt = lookAtSpot;


	// move camera
	lookAtSpot.x = transform->translationVector.x = minX + screenWidth * 0.5f;
	lookAtSpot.y = transform->translationVector.y = minY + screenHeight* 0.5f;
	transform->translationVector.z = dist;

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
		offset + transform->translationVector,	// Camera is here
		offset + lookAtSpot, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
		);
}

void FollowCamera::addTarget(ShiftKiddie * _target){
	targets.push_back(_target);
}
void FollowCamera::removeTarget(ShiftKiddie * _target){
	for(signed long int i = targets.size()-1; i >= 0; --i){
		if(targets.at(i) == _target){
			targets.erase(targets.begin() + i);
		}
	}
}