#pragma once

#include "CMD_KeyProperty.h"
#include "CMD_AddKeyframe.h"
#include "CMD_EditKeyframe.h"
#include "Tween.h"
#include "Animation.h"
#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_KeyProperty::CMD_KeyProperty(Animation * _animation, float _time, float _value, Easing::Type _interpolation) :
	animation(_animation),
	time(_time),
	value(_value),
	interpolation(_interpolation)
{
	int idx = findKeyframe(&animation->tweens);

	if(idx > 0){
		subCommands.push_back(new CMD_EditKeyframe(animation, value, interpolation, idx));
	}else{
		subCommands.push_back(new CMD_AddKeyframe(animation, time, value, interpolation));
	}
}

void CMD_KeyProperty::execute(){
	ci::app::console() << "execute CMD_KeyProperty" << std::endl;
	subCommands.at(0)->execute();
}

void CMD_KeyProperty::unexecute(){
	subCommands.at(0)->unexecute();
}

int CMD_KeyProperty::findKeyframe(std::vector<Tween> * _tweens){
	// find index of tween
	int idx = -1;
	float sumTime = 0;

	for(unsigned long int i = 0; i < _tweens->size(); ++i){
		sumTime += _tweens->at(i).deltaTime;
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
		value += animation->tweens.at(i).deltaValue;
	}

	return value;
}

float CMD_KeyProperty::getEndValue(int _idx){
	float value = animation->startValue;

	for(unsigned long int i = 0; i = _idx; ++i){
		value += animation->tweens.at(i).deltaValue;
	}

	return value;
}

CMD_KeyProperty::~CMD_KeyProperty(){
}