#pragma once

#include "CMD_KeyProperty.h"
#include "CMD_AddTween.h"
#include "CMD_EditTween.h"
#include "CMD_EditStartKey.h"
#include "Tween.h"
#include "Animation.h"
#include "Step.h"
#include "UI.h"
#include <algorithm>

#include <cinder/app/AppBasic.h>
<<<<<<< HEAD

CMD_KeyProperty::CMD_KeyProperty(Animation * _animation, float _currentTime, float _targetTime, float _value, Easing::Type _interpolation) :
	animation(_animation),
	currentTime(_currentTime),
	targetTime(_targetTime),
	value(_value),
	interpolation(_interpolation)
{
}

bool CMD_KeyProperty::execute(){
	// Executing for the first time, save the oldStartValue if keying 0, or create an add or edit command
	if(firstRun){
	    if(!animation->hasStart){
		    // If there are no keyframes and the start hasn't been set
            subCmdProc.executeCommand(new CMD_EditStartKey(animation, value, targetTime));
	    }else{
		    // subtract the change in time from the animation's time, if there has been any change
		    if((animation->currentAnimationTime - (UI::time - currentTime)) + (targetTime - currentTime) == 0){
			    subCmdProc.executeCommand(new CMD_EditStartKey(animation, value, targetTime));
		    }else{
			    // Edit or add a tween
			
				// find index of tween
				int idx = -1;
				float sumTime = animation->currentAnimationTime;
				if(sumTime + (targetTime - currentTime) > 0){
					for(unsigned long int i = 0; i < animation->tweens.size(); ++i){
						sumTime -= animation->tweens.at(i)->deltaTime;
						if(sumTime == (targetTime - currentTime)){
							idx = i;
							break;
						}
					}
				}
				ci::app::console() << "sumTime: " << sumTime << std::endl;
				if(idx >= 0){
					subCmdProc.executeCommand(new CMD_EditTween(animation, value, interpolation, idx));
				}else{
					subCmdProc.executeCommand(new CMD_AddTween(animation, currentTime, targetTime, value, interpolation));
				}
		    }
        }
	}else{
		subCmdProc.redo();
	}

	return true;
}

bool CMD_KeyProperty::unexecute(){
	subCmdProc.undo();

	return true;
}

float CMD_KeyProperty::getStartValue(unsigned long int _idx){
	
	float value = animation->startValue;

	for(unsigned long int i = 0; i < _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

float CMD_KeyProperty::getEndValue(unsigned long int _idx){
	float value = animation->startValue;

	for(unsigned long int i = 0; i = _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

CMD_KeyProperty::~CMD_KeyProperty(){
}
=======
>>>>>>> origin/E1-QuaternionAnimation
