#pragma once

#include "CMD_EditKeyframe.h"
#include "Keyframe.h"
#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_EditKeyframe::CMD_EditKeyframe(std::vector<Keyframe> * _keyframes, Keyframe * _keyframe, float _startValue, float _value, Easing::Type _interpolation) :
	keyframes(_keyframes),
	keyframe(_keyframe),
	startValue(_startValue),
	value(_value),
	interpolation(_interpolation),
	oldStartValue(_keyframe->startValue),
	oldValue(_keyframe->value),
	oldInterpolation(keyframe->interpolation),
	nextDeltaTime(NULL),
	nextDeltaValue(NULL)
{
	
	std::vector<Keyframe>::iterator followingKeyframe_it = std::upper_bound(keyframes->begin(),keyframes->end(),*keyframe,Keyframe::keyframe_compare);
	
	// Get startValue of next keyframe
	if(followingKeyframe_it != keyframes->end()){
		followingStartValue = followingKeyframe_it->startValue;
	}
}

void CMD_EditKeyframe::execute(){
	ci::app::console() << "execute CMD_EditKeyframe" << std::endl;
	keyframe->startValue = startValue;
	keyframe->value = value;
	keyframe->interpolation = interpolation;

	// Change next keyframe's start value, if this one's is changing
	if (value != oldValue){
		std::vector<Keyframe>::iterator followingKeyframe_it = std::upper_bound(keyframes->begin(),keyframes->end(),*keyframe,Keyframe::keyframe_compare);

		// Change next keyframe's start value
		if (followingKeyframe_it != keyframes->end()){
			followingKeyframe_it->startValue = keyframe->value;
		}
	}
}

void CMD_EditKeyframe::unexecute(){
	keyframe->startValue = oldStartValue;
	keyframe->value = oldValue;
	keyframe->interpolation = oldInterpolation;

	// Restore next keyframe's start value, if this one's is changing
	if (value != oldValue){
		std::vector<Keyframe>::iterator followingKeyframe_it = std::upper_bound(keyframes->begin(),keyframes->end(),*keyframe,Keyframe::keyframe_compare);

		if (followingKeyframe_it != keyframes->end()){
			followingKeyframe_it->startValue = followingStartValue;
		}
	}
}

int CMD_EditKeyframe::getNextTween(int _idx){
	// find index of next tween
	int idx = -1;

	if(tweens->size() > 0 && idx != tweens->size()){
		idx = idx + 1;
	}
	return idx;
}

float CMD_EditKeyframe::getTweenEndTime(int _idx){
	float time = 0;
	
	for(unsigned long int i = 0; i = _idx; ++i){
		time += tweens->at(i).deltaTime;
	}

	return time;
}

float CMD_EditKeyframe::getTweenEndValue(int _idx, float _startValue){
	float value = _startValue;

	for(unsigned long int i = 0; i = _idx; ++i){
		value += tweens->at(i).deltaValue;
	}

	return value;
}

CMD_EditKeyframe::~CMD_EditKeyframe()
{
}