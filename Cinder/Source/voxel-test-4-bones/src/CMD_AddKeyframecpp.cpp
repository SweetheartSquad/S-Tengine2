#pragma once

#include "CMD_AddKeyframe.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddKeyframe::CMD_AddKeyframe(Animation * _animation, float _time, float _value, Easing::Type _interpolation) :
	copyAnimation(Animation(* _animation)),
	tweens(&_animation->tweens),
	tween(Tween(0,0,_interpolation))
{
	// Get index of next tween, if it exists, else -1
	nextTweenIdx = getNextTween(_time);

	// Calculate deltaTime and deltaValue of new tween

	// delta time
	tween.deltaTime = _time;
	tween.deltaValue = _value;
	if(nextTweenIdx >= 0){
		// subtract from end time and value of tween before next tween, else they are time and value
		if(nextTweenIdx != 0){
			tween.deltaTime = _time - getTweenEndTime(nextTweenIdx - 1);
			tween.deltaValue = _value - getTweenEndValue(nextTweenIdx - 1, _animation->startValue);
		}
	}else{
		// subtract from end time and value of last tween
		tween.deltaTime = _time - getTweenEndTime(tweens->size() - 1);
		tween.deltaTime = _value - getTweenEndValue(tweens->size() - 1, _animation->startValue);
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

void CMD_AddKeyframe::execute(){
	ci::app::console() << "execute CMD_AddKeyframe" << std::endl;
	
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
}

void CMD_AddKeyframe::unexecute(){
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
}

int CMD_AddKeyframe::getNextTween(float _time){
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

float CMD_AddKeyframe::getTweenEndTime(int _idx){
	float time = 0;
	
	for(unsigned long int i = 0; i = _idx; ++i){
		time += tweens->at(i).deltaTime;
	}

	return time;
}

float CMD_AddKeyframe::getTweenEndValue(int _idx, float _startValue){
	float value = _startValue;

	for(unsigned long int i = 0; i = _idx; ++i){
		value += tweens->at(i).deltaValue;
	}

	return value;
}

CMD_AddKeyframe::~CMD_AddKeyframe()
{
}