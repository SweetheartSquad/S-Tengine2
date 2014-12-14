#pragma once

#include "CMD_AddTween.h"
#include "CMD_AddTweenBefore.h"
#include "CMD_AddTweenInside.h"
#include "CMD_AddTweenAfter.h"
#include "CMD_EditTween.h"
#include "CMD_EditStartKey.h"

#include "Step.h"

#include <cinder/app/AppBasic.h>

CMD_AddTween::CMD_AddTween(Animation * _animation, float _currentTime, float _targetTime, float _targetValue, Easing::Type _interpolation) :
	animation(_animation),
	deltaTimeline(_targetTime - _currentTime),
	targetValue(_targetValue),
	interpolation(_interpolation),
    oldCurrentTween(-1),
	newCurrentTween(-1),
	oldCurrentAnimationTime(0),
	newCurrentAnimationTime(0),
	oldCurrentTweenTime(0),
	newCurrentTweenTime(0),
	oldReferenceValue(0),
	newReferenceValue(0)
{	
}

bool CMD_AddTween::execute(){
	ci::app::console() << "execute CMD_AddTween" << std::endl;
    
	float newCurrTweenTime = 0;
	float newRefVal = 0;
    // calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){
		oldCurrentAnimationTime = animation->currentAnimationTime;
		oldCurrentTweenTime = animation->currentTweenTime;
		oldCurrentTween = animation->currentTween;
		oldReferenceValue = animation->referenceValue;

		float targetTime = animation->currentAnimationTime + deltaTimeline;

		if(targetTime <= 0){
            newCurrentTween = 0;
			subCmdProc.executeCommand(new CMD_AddTweenBefore(animation, deltaTimeline, targetValue, interpolation));
		}else{
			float sumTime = 0;
			bool insideAnimation = false;
			unsigned long int nextTweenIndex = -1;
			for(unsigned long int i = 0; i < animation->tweens.size(); ++i){
				sumTime += animation->tweens.at(i)->deltaTime;
				if(sumTime > targetTime){
					// Inside the animation
					insideAnimation = true;
					nextTweenIndex = i;
					break;
				}
			}

			if(insideAnimation){
               // newCurrentTween = nextTweenIndex;
				subCmdProc.executeCommand(new CMD_AddTweenInside(animation, deltaTimeline, targetValue, interpolation, nextTweenIndex));
			}else{
				// After the animation
                //newCurrentTween = animation->tweens.size();
				subCmdProc.executeCommand(new CMD_AddTweenAfter(animation, deltaTimeline, targetValue, interpolation, sumTime));
			}
		}
		Step s;
		s.setDeltaTime(animation->currentAnimationTime);
		animation->currentAnimationTime = 0;
		animation->currentTweenTime = 0;
		animation->currentTween = 0;
		animation->referenceValue = animation->startValue;
		animation->update(&s);

		// save vals
		newCurrentAnimationTime = animation->currentAnimationTime;
		newCurrentTweenTime = animation->currentTweenTime;
		newCurrentTween = animation->currentTween;
		newReferenceValue = animation->referenceValue;
	}else{
		subCmdProc.redo();
		
		animation->currentAnimationTime = newCurrentAnimationTime;
		animation->currentTweenTime = newCurrentTweenTime;
		animation->currentTween = newCurrentTween;
		animation->referenceValue = newReferenceValue;
	}

	return true;
}

bool CMD_AddTween::unexecute(){
	
	animation->currentAnimationTime = oldCurrentAnimationTime;
	animation->currentTweenTime = oldCurrentTweenTime;
	animation->currentTween = oldCurrentTween;
	animation->referenceValue = oldReferenceValue;

    subCmdProc.undo();
	return true;
}

CMD_AddTween::~CMD_AddTween(){
}