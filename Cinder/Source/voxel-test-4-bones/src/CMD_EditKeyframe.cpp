#pragma once

#include "CMD_EditKeyframe.h"
#include "Animation.h"
#include "Tween.h"
#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_EditKeyframe::CMD_EditKeyframe(Animation * _animation, float _value, Easing::Type _interpolation, int _idx) :
	tweens(&_animation->tweens),
	tween(&_animation->tweens.at(_idx)),
	interpolation(_interpolation)
{
	// Get index of next tween, if it exists, else -1
	nextTweenIdx = getNextTween(_idx);

	// Save old deltaTime(?) and deltaValue of edited tween
	oldDeltaValue = tween->deltaValue;

	// Calculate deltaTime(?) and deltaValue of edited tween
	tween->deltaValue = _value;
	if(nextTweenIdx >= 0){
		// subtract from end time and value of tween before next tween, else they are time and value
		if(nextTweenIdx != 0){
			tween->deltaValue = _value - getTweenEndValue(nextTweenIdx - 1, _animation->startValue);
		}
	}else{
		// subtract from end time(?) and value of last tween
		tween->deltaTime = _value - getTweenEndValue(tweens->size() - 1, _animation->startValue);
	}

	// save and calculate new delta times and values of next tween (being split)
	if(nextTweenIdx >= 0) {
		// get old values
		nextTween_oldDeltaValue = tweens->at(nextTweenIdx).deltaValue;
		// calculate new values
		nextTween_newDeltaValue = getTweenEndValue(nextTweenIdx, _animation->startValue) - _value;
	}
}

void CMD_EditKeyframe::execute(){
	ci::app::console() << "execute CMD_EditKeyframe" << std::endl;

	tween->deltaValue = deltaValue;
	tween->interpolation = interpolation;

	// Change next keyframe's start value, if this one's is changing
	if (deltaValue != oldDeltaValue){
		if (nextTweenIdx >= 0){
			tweens->at(nextTweenIdx).deltaValue = nextTween_newDeltaValue;
		}
	}
}

void CMD_EditKeyframe::unexecute(){
	tween->deltaValue = oldDeltaValue;
	tween->interpolation = oldInterpolation;

	// Restore next keyframe's start value, if this one's is changing
	if (deltaValue != oldDeltaValue){
		if (nextTweenIdx >= 0){
			tweens->at(nextTweenIdx).deltaValue = nextTween_oldDeltaValue;
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