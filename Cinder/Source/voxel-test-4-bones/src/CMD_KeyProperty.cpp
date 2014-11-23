#pragma once

#include "CMD_KeyProperty.h"
#include "CMD_AddTween.h"
#include "CMD_EditTween.h"
#include "Tween.h"
#include "Animation.h"
#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_KeyProperty::CMD_KeyProperty(Animation * _animation, float _time, float _value, Easing::Type _interpolation) :
	animation(_animation),
	time(_time),
	value(_value),
	interpolation(_interpolation),
	oldStartValue(NULL)
{
}

void CMD_KeyProperty::execute(){
	// Executing for the first time, save the oldStartValue if keying 0, or create an add or edit command
	if((subCommands.size() == 0) && (oldStartValue == NULL)){
		if(time == 0.f){
			oldStartValue = animation->startValue;
		}else{
			int idx = findKeyframe(&animation->tweens);

			if(idx >= 0){
				subCommands.push_back(new CMD_EditTween(animation, value, interpolation, idx));
			}else{
				subCommands.push_back(new CMD_AddTween(animation, time, value, interpolation));
			}
		}
	}
	ci::app::console() << "execute CMD_KeyProperty" << std::endl;
	if(subCommands.size() > 0){
		// add or edit keyframe
		subCommands.at(0)->execute();
	}else{
		// set start value
		animation->startValue = value;
	}
}

void CMD_KeyProperty::unexecute(){
	if(subCommands.size() > 0){
		subCommands.at(0)->unexecute();
	}else{
		animation->startValue = oldStartValue;
	}
}

int CMD_KeyProperty::findKeyframe(std::vector<Tween *> * _tweens){
	// find index of tween
	int idx = -1;
	float sumTime = 0;

	for(unsigned long int i = 0; i < _tweens->size(); ++i){
		sumTime += _tweens->at(i)->deltaTime;
		if(sumTime == time){
			idx = i;
			break;
		}
	}

	return idx;
}

float CMD_KeyProperty::getStartValue(int _idx){
	
	float value = animation->startValue;

	for(unsigned long int i = 0; i < _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

float CMD_KeyProperty::getEndValue(int _idx){
	float value = animation->startValue;

	for(unsigned long int i = 0; i = _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

CMD_KeyProperty::~CMD_KeyProperty(){
}