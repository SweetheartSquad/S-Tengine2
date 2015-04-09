#pragma once

#include "Entity.h"
#include "FollowCamera.h"
#include "System.h"
#include "Transform.h"
#include <algorithm>

FollowCamera::FollowCamera(float _buffer, glm::vec3 _offset, float _deadZoneX, float _deadZoneY, float _deadZoneZ):
	PerspectiveCamera(),
	NodeTransformable(new Transform()),
	//NodeAnimatable(),
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
	
	lastOrientation = transform->getOrientationQuat();
	glm::quat newOrientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	newOrientation = glm::rotate(newOrientation, yaw, upVectorLocal);
	newOrientation = glm::rotate(newOrientation, pitch, rightVectorLocal);

	newOrientation = glm::slerp(lastOrientation, newOrientation, 0.15f * static_cast<float>(vox::deltaTimeCorrection));
	transform->setOrientation(newOrientation);

	forwardVectorRotated   = newOrientation * forwardVectorLocal;
	rightVectorRotated	   = newOrientation * rightVectorLocal;
	upVectorRotated		   = newOrientation * upVectorLocal;
	
	lookAtSpot = glm::vec3(0.f, 0.f, 0.f);
	float targetMinX = 9999999999.f;
	float targetMinY = 9999999999.f;
	float targetMaxX = -9999999999.f;
	float targetMaxY = -9999999999.f;
	
	for(ShiftKiddie * nt : targets){
		glm::vec3 pos = nt->getPos(false);
		//lookAtSpot += pos;
		targetMinX = std::min(pos.x-buffer, targetMinX);
		targetMaxX = std::max(pos.x+buffer, targetMaxX);
		targetMinY = std::min(pos.y-buffer, targetMinY);
		targetMaxY = std::max(pos.y+buffer, targetMaxY);
	}

	for(signed long int i = interpolators.size()-1; i > 0; --i){
		if(i < targets.size()){
			interpolators.at(i) += (targets.at(i)->getPos(false) - interpolators.at(i))*1.f;
		}else{
			interpolators.at(i) += (transform->getTranslationVector() - interpolators.at(i))*0.01f;
		}

		glm::vec3 pos = interpolators.at(i);
		if(glm::distance(pos, transform->getTranslationVector()) < 1){
			interpolators.erase(interpolators.begin() + i);
			continue;
		}
		targetMinX = std::min(pos.x-buffer, targetMinX);
		targetMaxX = std::max(pos.x+buffer, targetMaxX);
		targetMinY = std::min(pos.y-buffer, targetMinY);
		targetMaxY = std::max(pos.y+buffer, targetMaxY);
	}

	float screenWidth = targetMaxX - targetMinX;
	float screenHeight = targetMaxY - targetMinY;

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
	lookAtSpot.x = targetMinX + screenWidth * 0.5f;
	lookAtSpot.y = targetMinY + screenHeight* 0.5f;
	
	transform->translate(lookAtSpot.x, lookAtSpot.y, dist, false);
}

glm::mat4 FollowCamera::getViewMatrix(){
	return glm::lookAt(
		offset + transform->getTranslationVector(),	// Camera is here
		offset + lookAtSpot, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
		);
}

void FollowCamera::addTarget(ShiftKiddie * _target){
	targets.push_back(_target);
	interpolators.push_back(glm::vec3(transform->getTranslationVector()));
}
void FollowCamera::removeTarget(ShiftKiddie * _target){
	for(signed long int i = targets.size()-1; i >= 0; --i){
		if(targets.at(i) == _target){
			targets.erase(targets.begin() + i);
		}
	}
}

bool FollowCamera::hasTarget(ShiftKiddie * _target){
	for(ShiftKiddie * sk : targets){
		if(sk == _target){
			return true;
		}
	}
	return false;
}