#pragma once

#include "ControllableOrthographicCamera.h"
#include "node/NodeUpdatable.h"
#include "Transform.h"

ControllableOrthographicCamera::ControllableOrthographicCamera(float left, float right, float bottom, float top, float near, float far):
	OrthographicCamera(left, right, bottom, top, near, far),
	NodeTransformable(new Transform()),
	NodeAnimatable(),
	NodeUpdatable(),
	target(nullptr),
	deadZone(2)
{
}

ControllableOrthographicCamera::~ControllableOrthographicCamera(){
}

void ControllableOrthographicCamera::update(Step* _step){
	OrthographicCamera::update(_step);
	if(target != nullptr){
		if(target->transform->translationVector.x > (abs(deadZone) + transform->translationVector.x)){
			transform->translationVector.x = target->transform->translationVector.x - deadZone;	
		}
		if(target->transform->translationVector.x <  transform->translationVector.x - abs(deadZone)){
			transform->translationVector.x = target->transform->translationVector.x + deadZone;	
		}
		if(target->transform->translationVector.y > (abs(deadZone) + transform->translationVector.y)){
			transform->translationVector.y = target->transform->translationVector.y - deadZone;			
		}	
		if(target->transform->translationVector.y < transform->translationVector.y - (abs(deadZone))){
			transform->translationVector.y = target->transform->translationVector.y + deadZone;		
		}
	}
}

void ControllableOrthographicCamera::follow(NodeTransformable* _nodeTransformable){
	target = _nodeTransformable;
}