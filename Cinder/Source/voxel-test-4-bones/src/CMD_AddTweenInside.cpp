#pragma once

#include "CMD_AddTweenInside.h"
#include "CMD_EditTween.h"
#include "CMD_EditStartKey.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddTweenInside::CMD_AddTweenInside(Animation * _animation, float _deltaTimeline, float _targetValue, Easing::Type _interpolation, unsigned long int _nextTweenIndex) :
	animation(_animation),
	deltaTimeline(_deltaTimeline),
	targetValue(_targetValue),
	tween(nullptr),
	interpolation(_interpolation),
	nextTweenIndex(_nextTweenIndex)
{	
}

bool CMD_AddTweenInside::execute(){
	ci::app::console() << "execute CMD_AddTweenInside" << std::endl;

	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;
		float deltaValue;

		// d2s for time and value are previous tween's time and value
		if(nextTweenIndex == 0){
			// Interrupting first tween
			deltaTime = targetTime;
			deltaValue = targetValue - animation->startValue;
		}else{
			// Interrupting a different tween
			deltaTime = targetTime - animation->getTweenEndTime(nextTweenIndex-1);
			deltaValue = targetValue - animation->getTweenEndValue(nextTweenIndex-1);
		}

		nextTween_oldDeltaTime = animation->tweens.at(nextTweenIndex)->deltaTime;
		nextTween_oldDeltaValue = animation->tweens.at(nextTweenIndex)->deltaValue;
		nextTween_newDeltaTime = animation->tweens.at(nextTweenIndex)->deltaTime - deltaTime;
		nextTween_newDeltaValue = animation->tweens.at(nextTweenIndex)->deltaValue - deltaValue;
		
		
		tween = new Tween(deltaTime, deltaValue, interpolation);
	}

	// insert tween
	animation->tweens.insert(animation->tweens.begin() + nextTweenIndex, tween);

	animation->tweens.at(nextTweenIndex+1)->deltaTime = nextTween_newDeltaTime;
	animation->tweens.at(nextTweenIndex+1)->deltaValue = nextTween_newDeltaValue;

	return true;
}

bool CMD_AddTweenInside::unexecute(){
	// Remove tween inside animation
	
	// after inserting, tween is in nextTweenIndex
	animation->tweens.erase(animation->tweens.begin() + nextTweenIndex);
	animation->tweens.at(nextTweenIndex+1)->deltaTime = nextTween_oldDeltaTime;
	animation->tweens.at(nextTweenIndex+1)->deltaValue = nextTween_oldDeltaValue;

	return true;
}

CMD_AddTweenInside::~CMD_AddTweenInside(){
	if(!executed){
		delete tween;
	}
	tween = nullptr;
}