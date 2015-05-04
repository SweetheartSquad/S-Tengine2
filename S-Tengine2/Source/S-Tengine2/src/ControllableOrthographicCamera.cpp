#pragma once

#include "ControllableOrthographicCamera.h"
#include "node/NodeUpdatable.h"
#include "Transform.h"

ControllableOrthographicCamera::ControllableOrthographicCamera(float left, float right, float bottom, float top, float near, float far):
	OrthographicCamera(left, right, bottom, top, near, far),
	target(nullptr),
	deadZone(2)
{
}

ControllableOrthographicCamera::~ControllableOrthographicCamera(){
}

void ControllableOrthographicCamera::update(Step* _step){
	OrthographicCamera::update(_step);
	if(target != nullptr){
		glm::vec3 targettv = target->parent->getTranslationVector();
		glm::vec3 tv = target->parent->getTranslationVector();

		if(targettv.x > (abs(deadZone) + tv.x)){
			tv.x = targettv.x - deadZone;	
		}
		if(targettv.x < tv.x - abs(deadZone)){
			tv.x = targettv.x + deadZone;	
		}
		if(targettv.y > (abs(deadZone) + tv.y)){
			tv.y = targettv.y - deadZone;			
		}	
		if(targettv.y < tv.y - (abs(deadZone))){
			tv.y = targettv.y + deadZone;		
		}

		parent->translate(tv, false);
	}
}

void ControllableOrthographicCamera::follow(Transform * _target){
	target = _target;
}