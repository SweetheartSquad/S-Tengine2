#pragma once

#include "CMD_EditTween.h"
#include "Animation.h"
#include "Tween.h"
#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_EditTween::CMD_EditTween(Animation * _animation, float _value, Easing::Type _interpolation, int _idx) :
	animation(_animation),
	value(_value),
	tween(_animation->tweens.at(_idx)),
	idx(_idx),
	interpolation(_interpolation),
	executed(false)
{
}

void CMD_EditTween::execute(){
	ci::app::console() << "execute CMD_EditTween" << std::endl;
	
	// save old tween values and calculate other values being affected by this tween edit
	if (!executed){
		// Get index of next tween, if it exists, else -1
		nextTweenIdx = getNextTween(idx);

		// Save old deltaTime(?) and deltaValue of edited tween
		oldDeltaValue = tween->deltaValue;

		// Calculate deltaValue of edited tween
		int prevTweenIdx = getPreviousTween(idx);
		deltaValue = value;
		if(prevTweenIdx >= 0){
			// d2 for value is previous tween's value
			deltaValue = value - getTweenEndValue(prevTweenIdx);
		}else{
			// d2 for value is start value
			deltaValue = value - animation->startValue;
		}

		// save and calculate new delta times and values of next tween (being split)
		if(nextTweenIdx >= 0) {
			// get old values
			nextTween_oldDeltaValue = animation->tweens.at(nextTweenIdx)->deltaValue;
			// calculate new values
			nextTween_newDeltaValue = getTweenEndValue(nextTweenIdx) - value;
		}
	}
	
	// Edit tween values
	tween->deltaValue = deltaValue;
	tween->interpolation = interpolation;

	// TODO: see if I can just create edit tween cmd now
	// Change next keyframe's start value, if this one's is changing
	if (deltaValue != oldDeltaValue){
		if (nextTweenIdx >= 0){
			animation->tweens.at(nextTweenIdx)->deltaValue = nextTween_newDeltaValue;
		}
	}

	executed = true;
}

void CMD_EditTween::unexecute(){
	tween->deltaValue = oldDeltaValue;
	tween->interpolation = oldInterpolation;

	// Restore next keyframe's start value, if this one's is changing
	if (deltaValue != oldDeltaValue){
		if (nextTweenIdx >= 0){
			animation->tweens.at(nextTweenIdx)->deltaValue = nextTween_oldDeltaValue;
		}
	}
}

int CMD_EditTween::getPreviousTween(int _idx){
	// find index of previous tween
	int idx = -1;

	if(animation->tweens.size() > 0 && _idx != 0){
		idx = _idx - 1;
	}
	return idx;
}

int CMD_EditTween::getNextTween(int _idx){
	// find index of next tween
	int idx = -1;

	if(animation->tweens.size() > 0 && _idx != animation->tweens.size() - 1){
		idx = _idx + 1;
	}
	return idx;
}

float CMD_EditTween::getTweenEndTime(int _idx){
	float time = 0;
	
	for(unsigned long int i = 0; i <= _idx; ++i){
		time += animation->tweens.at(i)->deltaTime;
	}

	return time;
}

float CMD_EditTween::getTweenEndValue(int _idx){
	float value = animation->startValue;

	for(unsigned long int i = 0; i <= _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

CMD_EditTween::~CMD_EditTween()
{
}