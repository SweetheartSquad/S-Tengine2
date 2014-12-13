#pragma once

#include "CMD_AddTween.h"
#include "CMD_AddTweenBefore.h"
#include "CMD_AddTweenInside.h"
#include "CMD_AddTweenAfter.h"
#include "CMD_EditTween.h"
#include "CMD_EditStartKey.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddTween::CMD_AddTween(Animation * _animation, float _currentTime, float _targetTime, float _targetValue, Easing::Type _interpolation) :
	animation(_animation),
	deltaTimeline(_targetTime - _currentTime),
	targetValue(_targetValue),
	interpolation(_interpolation),
    oldCurrentTween(-1)
{	
}

bool CMD_AddTween::execute(){
	ci::app::console() << "execute CMD_AddTween" << std::endl;
    
    // calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){
		oldCurrentTween = animation->currentTween;

		float targetTime = animation->time + deltaTimeline;

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

		// calculate the new currentTween
		newCurrentTween = animation->tweens.size()-1;
		float t = 0;
		float comparison = animation->time;
		while(comparison < 0){
			comparison += animation->getTweenEndTime(animation->tweens.size()-1);
		}while(comparison > animation->getTweenEndTime(animation->tweens.size()-1)){
			comparison -= animation->getTweenEndTime(animation->tweens.size()-1);
		}
		for(unsigned long int i = 0; i < animation->tweens.size(); ++i){
			t += animation->tweens.at(i)->deltaTime;
			if(t > comparison){
				newCurrentTween = i-1;
			}
		}
	}else{
		subCmdProc.redo();
	}
	
    animation->currentTween = newCurrentTween;


	return true;
}

bool CMD_AddTween::unexecute(){
    animation->currentTween = oldCurrentTween;
    subCmdProc.undo();
	return true;
}

CMD_AddTween::~CMD_AddTween(){
}