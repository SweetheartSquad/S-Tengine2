#pragma once

#include "CMD_AddTweenBefore.h"
#include "CMD_EditTween.h"
#include "CMD_EditStartKey.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddTweenBefore::CMD_AddTweenBefore(Animation * _animation, float _deltaTimeline, float _targetValue, Easing::Type _interpolation) :
	animation(_animation),
	deltaTimeline(_deltaTimeline),
	targetValue(_targetValue),
	tween(nullptr),
	interpolation(_interpolation)
{	
}

bool CMD_AddTweenBefore::execute(){
	ci::app::console() << "execute CMD_AddTweenBefore" << std::endl;
	
	CMD_EditStartKey * cmd = nullptr;

	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;// = deltaTimeline;
		float deltaValue;// = targetValue;

		if(targetTime <= 0){
			// Before the animation

			deltaTime = 0 - targetTime;
			// d2s for time and value are 0 and start value (deltaTime stays as time)
			deltaValue = animation->startValue - targetValue;

			cmd = new CMD_EditStartKey(animation, targetValue, targetTime);
		}
		tween = new Tween(deltaTime, deltaValue, interpolation);
	}

	// insert tween
	animation->tweens.insert(animation->tweens.begin(), tween);

	if(firstRun){
		subCmdProc.executeCommand(cmd);
	}else{
		subCmdProc.redo();
	}

	return true;
}

bool CMD_AddTweenBefore::unexecute(){
    subCmdProc.undo();
	// Remove tween before animation
	animation->tweens.erase(animation->tweens.begin());
	return true;
}

CMD_AddTweenBefore::~CMD_AddTweenBefore(){
	if(!executed){
		delete tween;
	}
	tween = nullptr;
}