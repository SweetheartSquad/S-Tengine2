#pragma once

#include "Entity.h"
#include "FollowCamera.h"
#include "Transform.h"
#include <algorithm>

FollowCamera::FollowCamera(float _buffer, glm::vec3 _offset, float _deadZoneX, float _deadZoneY, float _deadZoneZ):
	buffer(_buffer),
	lastOrientation(1.f, 0.f, 0.f, 0.f),
	offset(_offset),
	deadZoneX(_deadZoneX),
	deadZoneY(_deadZoneY),
	deadZoneZ(_deadZoneZ),
	minimumZoom(10),
	minBounds(0,0,0,0),
	useBounds(false)
{
}

FollowCamera::~FollowCamera(){
}

void FollowCamera::update(Step * _step){
	
	lastOrientation = childTransform->getOrientationQuat();
	glm::quat newOrientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	newOrientation = glm::rotate(newOrientation, yaw, upVectorLocal);
	newOrientation = glm::rotate(newOrientation, pitch, rightVectorLocal);

	newOrientation = glm::slerp(lastOrientation, newOrientation, 0.15f * static_cast<float>(sweet::deltaTimeCorrection));
	childTransform->setOrientation(newOrientation);

	forwardVectorRotated   = newOrientation * forwardVectorLocal;
	rightVectorRotated	   = newOrientation * rightVectorLocal;
	upVectorRotated		   = newOrientation * upVectorLocal;
	
	lookAtSpot = glm::vec3(0.f, 0.f, 0.f);
	float targetMinX = 9999999999.f;
	float targetMinY = 9999999999.f;
	float targetMaxX = -9999999999.f;
	float targetMaxY = -9999999999.f;
	
	for(signed long int i = targets.size()-1; i >= 0; --i){
		if(!targets.at(i).active){
			if(targets.at(i).weight <= 0.001f){
				targets.erase(targets.begin() + i);
			}
		}else{
			targets.at(i).pos = targets.at(i).target->getWorldPos();
		}
	}

	for(Target & t : targets){
		targetMinX = std::min((t.pos.x-buffer)*t.weight, targetMinX);
		targetMaxX = std::max((t.pos.x+buffer)*t.weight, targetMaxX);
		targetMinY = std::min((t.pos.y-buffer)*t.weight, targetMinY);
		targetMaxY = std::max((t.pos.y+buffer)*t.weight, targetMaxY);

		if(t.active){
			t.weight = std::min(1.f, t.weight + 0.05f);
		}else{
			t.weight = std::max(0.f, t.weight - 0.01f);
		}
	}

	float screenWidth = targetMaxX - targetMinX;
	float screenHeight = targetMaxY - targetMinY;
	
	// move camera
	lookAtSpot.x = targetMinX;
	lookAtSpot.y = targetMinY;
	
	lookAtSpot += offset;
	
	if(useBounds){
		if(minBounds.height != 0){
			if(lookAtSpot.y < minBounds.y){
				lookAtSpot.y = minBounds.y;
			}
			if(lookAtSpot.y + screenHeight > minBounds.x + minBounds.height){
				lookAtSpot.y -= (lookAtSpot.y + screenHeight - (minBounds.y + minBounds.height));
			}
			if(lookAtSpot.y < minBounds.y){
				screenHeight -= minBounds.y - lookAtSpot.y;
				lookAtSpot.y = minBounds.y;
			}
		}

		if(minBounds.width != 0){
			if(lookAtSpot.x < minBounds.x){
				lookAtSpot.x = minBounds.x;
			}
			if(lookAtSpot.x + screenWidth > minBounds.x + minBounds.width){
				lookAtSpot.x -= (lookAtSpot.x + screenWidth - (minBounds.x + minBounds.width));
			}
			if(lookAtSpot.x < minBounds.x){
				screenWidth -= minBounds.x - lookAtSpot.x;
				lookAtSpot.x = minBounds.x;
			}
		}
	}

	// calculate zoom and account for FoV (the camera FoV seems to be vertical, so if the screen w > screen h, we need to take the h / the intended aspect ratio)
	float ar1 = screenWidth/screenHeight;
	glm::vec2 screenDimensions = sweet::getWindowDimensions();
	float ar2 = static_cast<float>(screenDimensions.x)/static_cast<float>(screenDimensions.y);
	float zoom;
	if(ar1 > ar2){
		zoom = std::max(minimumZoom, screenWidth / ar2);
	}else if(ar1 < ar2){
		zoom = std::max(minimumZoom, screenHeight);
	}else{
		zoom = std::max(minimumZoom, screenHeight);
	}

	lookAtSpot.x += screenWidth * 0.5f;
	lookAtSpot.y += screenHeight * 0.5f;

	lookAtSpot += offset;

	float dist = zoom / (tan(glm::radians(fieldOfView) * 0.5f) * 2.f);


	parents.at(0)->translate(lookAtSpot.x, lookAtSpot.y, dist, false);
}

void FollowCamera::addTarget(NodeChild * _target, float _weight){
	assert(_target != nullptr);
	Target t;
	t.target = _target;
	t.weight = _weight;
	t.active = true;
	targets.push_back(t);
}
void FollowCamera::removeTarget(NodeChild * _target){
	for(signed long int i = targets.size()-1; i >= 0; --i){
		if(targets.at(i).target == _target){
			targets.at(i).active = false;
		}
	}
}

bool FollowCamera::hasTarget(NodeChild * _target){
	for(Target & t : targets){
		if(t.target == _target){
			return true;
		}
	}
	return false;
}