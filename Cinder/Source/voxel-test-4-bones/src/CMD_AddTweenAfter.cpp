#pragma once

#include "CMD_AddTweenAfter.h"
#include "CMD_EditTween.h"
#include "CMD_EditStartKey.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddTweenAfter::CMD_AddTweenAfter(Animation * _animation, float _deltaTimeline, float _targetValue, Easing::Type _interpolation, float _sumTime) :
	animation(_animation),
	deltaTimeline(_deltaTimeline),
	targetValue(_targetValue),
	tween(nullptr),
	interpolation(_interpolation),
    sumTime(_sumTime)
{	
}

bool CMD_AddTweenAfter::execute(){
	ci::app::console() << "execute CMD_AddTweenAfter" << std::endl;

	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;// = deltaTimeline;
		float deltaValue;// = targetValue;

		// After the animation
		deltaTime = targetTime - sumTime;
		if(animation->tweens.size() > 0){
			deltaValue = targetValue - animation->getTweenEndValue(animation->tweens.size()-1);
		}else{
			deltaValue = targetValue - animation->startValue;
		}
		
		tween = new Tween(deltaTime, deltaValue, interpolation);
	}
	
	// insert tween
	animation->tweens.insert(animation->tweens.end(), tween);
	return true;
}

bool CMD_AddTweenAfter::unexecute(){
	// Remove tween after animation
	animation->tweens.erase(animation->tweens.begin() + animation->tweens.size() - 1);
	return true;
}

CMD_AddTweenAfter::~CMD_AddTweenAfter(){
	if(!executed){
		delete tween;
	}
	tween = nullptr;
}