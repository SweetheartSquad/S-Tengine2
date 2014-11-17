#pragma once

#include "CMD_AddTween.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddTween::CMD_AddTween(Animation * _animation, float _time, float _value, Easing::Type _interpolation) :
	animation(_animation),
	startValue(NULL),
	time(_time),
	value(_value),
	executed(false),
	tween(nullptr),
	interpolation(_interpolation)
{
	// Get index of next tween, if it exists, else -1
	
}

void CMD_AddTween::execute(){
	ci::app::console() << "execute CMD_AddTween" << std::endl;
	
	

	nextTweenIdx = getNextTween(time);

	// Calculate deltaTime and deltaValue of new tween
	float deltaTime = time;
	float deltaValue = value;
	if(nextTweenIdx >= 0){
		// subtract from end time and value of tween before next tween, else they are time and value
		if(nextTweenIdx != 0){
			deltaTime = time - getTweenEndTime(nextTweenIdx - 1);
			deltaValue = value - getTweenEndValue(nextTweenIdx - 1, animation->startValue);
		}
	}else{
		// subtract from end time and value of last tween, if there are any, else they are time and value
		if(animation->tweens.size() > 0){
			deltaTime = time - getTweenEndTime(animation->tweens.size() - 1);
			deltaValue = value - getTweenEndValue(animation->tweens.size() - 1, animation->startValue);
		}
	}

	// save and calculate new delta times and values of next tween (being split)
	if(nextTweenIdx >= 0) {
		// get old values
		nextTween_oldDeltaTime = animation->tweens.at(nextTweenIdx)->deltaTime;
		nextTween_oldDeltaValue = animation->tweens.at(nextTweenIdx)->deltaValue;
		// calculate new values
		nextTween_newDeltaTime = animation->tweens.at(nextTweenIdx)->deltaTime - deltaTime;
		nextTween_newDeltaValue = getTweenEndValue(nextTweenIdx, animation->startValue) - value;
	}

	// Add keyframe
	std::vector<Tween *>::iterator nextTween_it;
	if(nextTweenIdx >= 0){
		nextTween_it = animation->tweens.begin() + nextTweenIdx;
	}else{
		nextTween_it = animation->tweens.end();
	}
	

	if(nextTweenIdx >=0){
		// Get the new iterator from the new position after insert
		nextTween_it = animation->tweens.begin() + nextTweenIdx + 1;

		// d2 - d1
		(*nextTween_it)->deltaTime = nextTween_newDeltaTime;
		(*nextTween_it)->deltaValue = nextTween_newDeltaValue;
	}

	// If we just keyed time 0, update startValue of animation
	if(startValue != NULL){
		animation->startValue = startValue;
	}

	if(tween == nullptr){
		tween = new Tween(deltaTime, deltaValue, interpolation);
	}
	animation->tweens.insert(nextTween_it, tween);
}

void CMD_AddTween::unexecute(){
	// Remove keyframe

	if(nextTweenIdx >= 0){
		// after inserting, tween is in nextTweenIdx
		animation->tweens.erase(animation->tweens.begin() + nextTweenIdx);
	}else{
		animation->tweens.erase(animation->tweens.begin() + animation->tweens.size() - 1);
	}

	if(nextTweenIdx >=0){
		// after erasing, nextTweenIdx is the actual next tween
		std::vector<Tween *>::iterator nextTween_it = animation->tweens.begin() + nextTweenIdx;

		// Restore old delta time and value
		(*nextTween_it)->deltaTime = nextTween_oldDeltaTime;
		(*nextTween_it)->deltaValue = nextTween_oldDeltaValue;
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

	for(unsigned long int i = 0; i < animation->tweens.size(); ++i){
		sumTime += animation->tweens.at(i)->deltaTime;
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
		time += animation->tweens.at(i)->deltaTime;
	}

	return time;
}

float CMD_AddTween::getTweenEndValue(int _idx, float _startValue){
	float value = _startValue;

	for(unsigned long int i = 0; i <= _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

CMD_AddTween::~CMD_AddTween()
{
}