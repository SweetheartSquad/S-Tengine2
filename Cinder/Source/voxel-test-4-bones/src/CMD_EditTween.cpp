#pragma once

#include "CMD_EditTween.h"
#include "Animation.h"
#include "Tween.h"
#include <algorithm>


#include <cinder/app/AppBasic.h>

CMD_EditTween::CMD_EditTween(Animation * _animation, float _targetValue, Easing::Type _interpolation, int _idx) :
	animation(_animation),
	targetValue(_targetValue),
	tween(_animation->tweens.at(_idx)),
	idx(_idx),
	interpolation(_interpolation)
{
}

void CMD_EditTween::execute(){
	ci::app::console() << "execute CMD_EditTween" << std::endl;
	// save old tween values and calculate other values being affected by this tween edit
	if (!executed){
		// Save old deltaValue of edited tween
		oldDeltaValue = tween->deltaValue;

		// Get index of next tween, if it exists, else -1
		nextTweenIndex = idx == animation->tweens.size()-1 ? -1 : idx + 1;
		if(nextTweenIndex < animation->tweens.size()){
			nextTween_oldDeltaValue = animation->tweens.at(nextTweenIndex)->deltaValue;
			nextTween_newDeltaValue = nextTween_oldDeltaValue + (newDeltaValue - oldDeltaValue);
		}

		// Calculate deltaValue of edited tween
		if(idx > 0){
			// d2 for value is previous tween's value
			newDeltaValue = targetValue - animation->getTweenEndValue(idx-1);
		}else{
			// d2 for value is start value
			newDeltaValue = targetValue - animation->startValue;
		}
	}
	
	// Edit tween values
	tween->deltaValue = newDeltaValue;
	tween->interpolation = interpolation;

	// TODO: see if I can just create edit tween cmd now
	// Change next keyframe's start value, if this one's is changing
	if (newDeltaValue != oldDeltaValue){
		if (nextTweenIndex < animation->tweens.size()){
			animation->tweens.at(nextTweenIndex)->deltaValue = nextTween_newDeltaValue;
		}
	}
}

void CMD_EditTween::unexecute(){
	tween->deltaValue = oldDeltaValue;
	tween->interpolation = oldInterpolation;

	// Restore next keyframe's start value, if this one's is changing
	if (newDeltaValue != oldDeltaValue){
		if (nextTweenIndex < animation->tweens.size()){
			animation->tweens.at(nextTweenIndex)->deltaValue = nextTween_oldDeltaValue;
		}
	}
}

CMD_EditTween::~CMD_EditTween()
{
}