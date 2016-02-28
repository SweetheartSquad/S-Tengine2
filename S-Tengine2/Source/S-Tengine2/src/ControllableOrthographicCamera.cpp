#pragma once

#include "ControllableOrthographicCamera.h"
#include "node/NodeUpdatable.h"
#include "Transform.h"

ControllableOrthographicCamera::ControllableOrthographicCamera(float _left, float _right, float _bottom, float _top, float _near, float _far):
	OrthographicCamera(_left, _right, _bottom, _top, _near, _far),
	target(nullptr),
	deadZone(2)
{
}

ControllableOrthographicCamera::~ControllableOrthographicCamera(){
}

void ControllableOrthographicCamera::update(Step* _step){
	OrthographicCamera::update(_step);
	if(target != nullptr){
		glm::vec3 targettv = target->parents.at(0)->getTranslationVector();
		glm::vec3 tv = target->parents.at(0)->getTranslationVector();

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

		parents.at(0)->translate(tv, false);
	}
}

void ControllableOrthographicCamera::follow(Transform * _target){
	target = _target;
}