#pragma once

#include "CMD_EditKeyframe.h"
#include "Animation.h"
#include "Tween.h"
#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_EditKeyframe::CMD_EditKeyframe(Animation * _animation, float _value, Easing::Type _interpolation, int _idx) :
	animation(_animation),
	tweens(&_animation->tweens),
	tween(&_animation->tweens.at(_idx)),
	interpolation(_interpolation),
	startValue(NULL)
{
	// Get index of next tween, if it exists, else -1
	nextTweenIdx = getNextTween(_idx);

	// Save old deltaTime(?) and deltaValue of edited tween
	oldDeltaValue = tween->deltaValue;

	// Calculate deltaTime(?) and deltaValue of edited tween
	deltaValue = _value;
	if(nextTweenIdx >= 0){
		// subtract from end time and value of tween before this tween, else they are time and value
		if(nextTweenIdx != 0){
			deltaValue = _value - getTweenEndValue(_idx - 1, _animation->startValue);
		}
	}else{
		// subtract from end time and value of last tween, if there are any, else they are time and value
		if(tweens->size() > 0){
			deltaValue = _value - getTweenEndValue(tweens->size() - 1, _animation->startValue);
		}
	}

	//// if we edit time 0, deltaValue should be 0? set startValue?
	if(getTweenEndTime(_idx) == 0){
		startValue = _value;
		oldStartValue = _animation->startValue;
		deltaValue = 0;
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

	// If we just keyed time 0, update startValue of animation
	if(startValue != NULL){
		animation->startValue = startValue;
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

	// If we undid keying time 0, restore startValue of animation
	if(startValue != NULL){
		animation->startValue = oldStartValue;
	}
}

int CMD_EditKeyframe::getNextTween(int _idx){
	// find index of next tween
	int idx = -1;

	if(tweens->size() > 0 && idx != tweens->size()){
		idx = _idx + 1;
	}
	return idx;
}

float CMD_EditKeyframe::getTweenEndTime(int _idx){
	float time = 0;
	
	for(unsigned long int i = 0; i <= _idx; ++i){
		time += tweens->at(i).deltaTime;
	}

	return time;
}

float CMD_EditKeyframe::getTweenEndValue(int _idx, float _startValue){
	float value = _startValue;

	for(unsigned long int i = 0; i <= _idx; ++i){
		value += tweens->at(i).deltaValue;
	}

	return value;
}

CMD_EditKeyframe::~CMD_EditKeyframe()
{
}