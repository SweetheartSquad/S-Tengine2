#pragma once

#include "CMD_KeyProperty.h"
#include "CMD_AddTween.h"
#include "CMD_EditTween.h"
#include "Tween.h"
#include "Animation.h"
#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_KeyProperty::CMD_KeyProperty(Animation * _animation, float _currentTime, float _targetTime, float _value, Easing::Type _interpolation) :
	animation(_animation),
	currentTime(_currentTime),
	targetTime(_targetTime),
	value(_value),
	interpolation(_interpolation),
	executed(false)
{
}

void CMD_KeyProperty::execute(){
	// Executing for the first time, save the oldStartValue if keying 0, or create an add or edit command
	if(!animation->hasStart){
		// If there are no keyframes and the start hasn't been set
		oldStartValue = animation->startValue;

		// set start value
		animation->startValue = value;
		animation->referenceValue = value;
		animation->hasStart = true;
		oldHasStart = false;
	}else{
		if(animation->time + (targetTime - currentTime) == 0){
			// Edit start of the animation
			oldStartValue = animation->startValue;
			oldHasStart = true;

			// set start value
			animation->startValue = value;
			animation->referenceValue = value;

			if(animation->tweens.size() > 0){
				// If there are other keyframes
				animation->tweens.at(0)->deltaValue -= (value - oldStartValue);
			}
		}else{
			// Edit or add a tween
			if(subCommands.size() == 0){
				// find index of tween
				int idx = -1;
				float sumTime = animation->time;
				for(unsigned long int i = 0; i < animation->tweens.size(); ++i){
					sumTime += animation->tweens.at(i)->deltaTime;
					if(sumTime == (targetTime - currentTime)){
						idx = i;
						break;
					}
				}
				if(idx >= 0){
					subCommands.push_back(new CMD_EditTween(animation, value, interpolation, idx));
				}else{
					subCommands.push_back(new CMD_AddTween(animation, currentTime, targetTime, value, interpolation));
				}
			}
			subCommands.at(0)->execute();
		}
	}

	executed = true;
}

void CMD_KeyProperty::unexecute(){
	if(subCommands.size() > 0){
		subCommands.at(0)->unexecute();
	}else{
		animation->startValue = oldStartValue;
		animation->hasStart = oldHasStart;
		if(oldHasStart == true || animation->tweens.size() > 0){
			animation->tweens.at(0)->deltaValue += (value - oldStartValue);
		}
	}

	executed = false;
}

float CMD_KeyProperty::getStartValue(unsigned long int _idx){
	
	float value = animation->startValue;

	for(unsigned long int i = 0; i < _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

float CMD_KeyProperty::getEndValue(unsigned long int _idx){
	float value = animation->startValue;

	for(unsigned long int i = 0; i = _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

CMD_KeyProperty::~CMD_KeyProperty(){
}