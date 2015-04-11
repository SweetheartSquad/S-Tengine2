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
	minimumZoom(10),
	minBounds(0,0,0,0),
	useBounds(false)
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
	
	for(signed long int i = targets.size()-1; i >= 0; --i){
		if(!targets.at(i).active){
			if(targets.at(i).weight <= 0.001f){
				targets.erase(targets.begin() + i);
			}
		}else{
			targets.at(i).pos = targets.at(i).target->getPos(false);
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
	
	//glm::vec3 oldLookAt = lookAtSpot;

	// move camera
	lookAtSpot.x = targetMinX;// + screenWidth * 0.5f;
	lookAtSpot.y = targetMinY;// + screenHeight* 0.5f;
	
	lookAtSpot += offset;


	//std::cout << lookAtSpot.x << " " << screenWidth << std::endl;
	
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
		//
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


	//std::cout << lookAtSpot.x << " " << screenWidth << std::endl << std::endl;
	
	// calculate zoom and account for FoV (the camera FoV seems to be vertical, so if the screen w > screen h, we need to take the h / the intended aspect ratio)
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

	lookAtSpot.x += screenWidth * 0.5f;
	lookAtSpot.y += screenHeight * 0.5f;

	//std::cout << lookAtSpot.x << " " << screenWidth << std::endl << std::endl;

	float dist = zoom / (tan(glm::radians(fieldOfView) * 0.5f) * 2.f);


	transform->translate(lookAtSpot.x, lookAtSpot.y, dist, false);
	transform->translate(offset);
}

glm::mat4 FollowCamera::getViewMatrix(){
	return glm::lookAt(
		/*offset + */transform->getTranslationVector(),	// Camera is here
		/*offset + */lookAtSpot, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
		);
}

void FollowCamera::addTarget(ShiftKiddie * _target, float _weight){
	Target t;
	t.target = _target;
	t.weight = _weight;
	t.active = true;
	targets.push_back(t);
}
void FollowCamera::removeTarget(ShiftKiddie * _target){
	for(signed long int i = targets.size()-1; i >= 0; --i){
		if(targets.at(i).target == _target){
			//targets.erase(targets.begin() + i);
			targets.at(i).active = false;
		}
	}
}

bool FollowCamera::hasTarget(ShiftKiddie * _target){
	for(Target & t : targets){
		if(t.target == _target){
			return true;
		}
	}
	return false;
}