#pragma once

#include "CMD_AddTween.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddTween::CMD_AddTween(Animation * _animation, float _time, float _value, Easing::Type _interpolation) :
	animation(_animation),
	time(_time),
	value(_value),
	tween(nullptr),
	interpolation(_interpolation)
{	
}

void CMD_AddTween::execute(){
	ci::app::console() << "execute CMD_AddTween" << std::endl;
	
	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (tween == nullptr){
		// Get index of next tween, if it exists, else -1
		nextTweenIdx = getNextTween(time);

		// Calculate deltaTime and deltaValue of new tween, where d1s are time and value
		int prevTweenIdx = getPreviousTween(nextTweenIdx);
		float deltaTime = time;
		float deltaValue = value;
		if(prevTweenIdx >= 0){
			// d2s for time and value are previous tween's time and value
			deltaTime = time - getTweenEndTime(prevTweenIdx);
			deltaValue = value - getTweenEndValue(prevTweenIdx, animation->startValue);
		}else{
			// d2s for time and value are 0 and start value (deltaTime stays as time)
			deltaValue = value - animation->startValue;
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
		
		tween = new Tween(deltaTime, deltaValue, interpolation);
	}

	// Get insert iterator
	std::vector<Tween *>::iterator nextTween_it;
	if(nextTweenIdx >= 0){
		nextTween_it = animation->tweens.begin() + nextTweenIdx;
	}else{
		nextTween_it = animation->tweens.end();
	}
	
	// insert tween
	animation->tweens.insert(nextTween_it, tween);

	// TODO: see if I can just call edit tween cmd now
	// update next tween's delta time and delta value, if it exists
	if(nextTweenIdx >=0){
		// Get the new iterator from the new position after insert
		nextTween_it = animation->tweens.begin() + nextTweenIdx + 1;

		// d2 - d1
		(*nextTween_it)->deltaTime = nextTween_newDeltaTime;
		(*nextTween_it)->deltaValue = nextTween_newDeltaValue;
	}
}

void CMD_AddTween::unexecute(){

	// Remove tween
	if(nextTweenIdx >= 0){
		// after inserting, tween is in nextTweenIdx
		animation->tweens.erase(animation->tweens.begin() + nextTweenIdx);
	}else{
		animation->tweens.erase(animation->tweens.begin() + animation->tweens.size() - 1);
	}

	// restore next tween's delta time and delta value, if it exists
	if(nextTweenIdx >=0){
		// after erasing, nextTweenIdx is the actual next tween
		std::vector<Tween *>::iterator nextTween_it = animation->tweens.begin() + nextTweenIdx;

		// Restore old delta time and value
		(*nextTween_it)->deltaTime = nextTween_oldDeltaTime;
		(*nextTween_it)->deltaValue = nextTween_oldDeltaValue;
	}
}

int CMD_AddTween::getPreviousTween(int _idx){
	// find index of previous tween
	int idx = -1;

	if(animation->tweens.size() > 0){
		// if idx is not -1
		if(_idx >= 0){
			if(_idx != 0){
				idx = _idx - 1;
			}
		}else{
			// previous tween is last tween
			idx = animation->tweens.size() - 1;
		}
		
	}
	return idx;
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