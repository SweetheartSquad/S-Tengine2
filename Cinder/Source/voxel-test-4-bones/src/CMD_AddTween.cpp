#pragma once

#include "CMD_AddTween.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddTween::CMD_AddTween(Animation * _animation, float _time, float _value, Easing::Type _interpolation) :
	animation(_animation),
	tweens(&_animation->tweens),
	tween(Tween(0,0,_interpolation)),
	startValue(NULL)
{
	// Get index of next tween, if it exists, else -1
	nextTweenIdx = getNextTween(_time);

	// Calculate deltaTime and deltaValue of new tween
	tween.deltaTime = _time;
	tween.deltaValue = _value;
	if(nextTweenIdx >= 0){
		// subtract from end time and value of tween before next tween, else they are time and value
		if(nextTweenIdx != 0){
			tween.deltaTime = _time - getTweenEndTime(nextTweenIdx - 1);
			tween.deltaValue = _value - getTweenEndValue(nextTweenIdx - 1, _animation->startValue);
		}
	}else{
		// subtract from end time and value of last tween, if there are any, else they are time and value
		if(tweens->size() > 0){
			tween.deltaTime = _time - getTweenEndTime(tweens->size() - 1);
			tween.deltaValue = _value - getTweenEndValue(tweens->size() - 1, _animation->startValue);
		}
	}

	//// if we key time 0, deltaValue should be 0? set startValue?
	if(_time == 0){
		startValue = _value;
		oldStartValue = _animation->startValue;
		tween.deltaValue = 0;
	}

	// save and calculate new delta times and values of next tween (being split)
	if(nextTweenIdx >= 0) {
		// get old values
		nextTween_oldDeltaTime = tweens->at(nextTweenIdx).deltaTime;
		nextTween_oldDeltaValue = tweens->at(nextTweenIdx).deltaValue;
		// calculate new values
		nextTween_newDeltaTime = tweens->at(nextTweenIdx).deltaTime - tween.deltaTime;
		nextTween_newDeltaValue = getTweenEndValue(nextTweenIdx, _animation->startValue) - _value;
	}
}

void CMD_AddTween::execute(){
	ci::app::console() << "execute CMD_AddTween" << std::endl;
	
	// Add keyframe
	std::vector<Tween>::iterator nextTween_it;
	if(nextTweenIdx >= 0){
		nextTween_it = tweens->begin() + nextTweenIdx;
	}else{
		nextTween_it = tweens->end();
	}
	
	tweens->insert(nextTween_it,tween);

	if(nextTweenIdx >=0){
		// Get the new iterator from the new position after insert
		nextTween_it = tweens->begin() + nextTweenIdx + 1;

		// d2 - d1
		nextTween_it->deltaTime = nextTween_newDeltaTime;
		nextTween_it->deltaValue = nextTween_newDeltaValue;
	}

	// If we just keyed time 0, update startValue of animation
	if(startValue != NULL){
		animation->startValue = startValue;
	}
}

void CMD_AddTween::unexecute(){
	// Remove keyframe

	if(nextTweenIdx >= 0){
		// after inserting, tween is in nextTweenIdx
		tweens->erase(tweens->begin() + nextTweenIdx);
	}else{
		tweens->erase(tweens->begin() + tweens->size() - 1);
	}

	if(nextTweenIdx >=0){
		// after erasing, nextTweenIdx is the actual next tween
		std::vector<Tween>::iterator nextTween_it = tweens->begin() + nextTweenIdx;

		// Restore old delta time and value
		nextTween_it->deltaTime = nextTween_oldDeltaTime;
		nextTween_it->deltaValue = nextTween_oldDeltaValue;
	}

	// If we undid keying time 0, restore startValue of animation
	if(startValue != NULL){
		animation->startValue = oldStartValue;
	}
}

int CMD_AddTween::getNextTween(float _time){
	// find index of next tween
	int idx = -1;
	float sumTime = 0;

	for(unsigned long int i = 0; i < tweens->size(); ++i){
		sumTime += tweens->at(i).deltaTime;
		if(sumTime > _time){
			idx = i;
			break;
		}
	}
	return idx;
}

float CMD_AddTween::getTweenEndTime(int _idx){
	float time = 0;
	
	for(unsigned long int i = 0; i <= _idx; ++i){
		time += tweens->at(i).deltaTime;
	}

	return time;
}

float CMD_AddTween::getTweenEndValue(int _idx, float _startValue){
	float value = _startValue;

	for(unsigned long int i = 0; i <= _idx; ++i){
		value += tweens->at(i).deltaValue;
	}

	return value;
}

CMD_AddTween::~CMD_AddTween()
{
}