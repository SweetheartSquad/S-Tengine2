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

CMD_KeyProperty::CMD_KeyProperty(Animation * _animation, float _currentTime, float _targetTime, float _value, Easing::Type _interpolation) :
	animation(_animation),
	currentTime(_currentTime),
	targetTime(_targetTime),
	value(_value),
	interpolation(_interpolation)
{
}

void CMD_KeyProperty::execute(){
	// Executing for the first time, save the oldStartValue if keying 0, or create an add or edit command
	if(firstRun){
        oldReferenceValue = animation->referenceValue;
	    if(!animation->hasStart){
		    // If there are no keyframes and the start hasn't been set
            subCmdProc.executeCommand(new CMD_EditStartKey(animation, value, targetTime));
	    }else{
		    // subtract the change in time from the animation's time, if there has been any change
		    if((animation->time - (UI::time - currentTime)) + (targetTime - currentTime) == 0){
			    subCmdProc.executeCommand(new CMD_EditStartKey(animation, value, targetTime));
		    }else{
			    // Edit or add a tween
			
				// find index of tween
				int idx = -1;
				float sumTime = animation->time;
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
	// We need to update the animation's time and reference value to the current time (not necessarily the same time as first run) using the new tweens/start value somehow
	
    // Now that we have a time command, we can just update the reference value to the value and the current tween to the index
	/*Step s;
	s.setDeltaTime(animation->time);
	ci::app::console() << "step deltaTime: " << animation->time << std::endl;
	animation->time = 0;
	animation->referenceValue = animation->startValue;
	animation->currentTime = 0;
	animation->currentTween = 0;
		
	animation->update(&s);
    */
    animation->referenceValue = value;
}

void CMD_KeyProperty::unexecute(){

	subCmdProc.undo();

	// We need to update the animation's time and reference value to the current time (not necessarily the same time as first run) using the restored tweens/start value somehow
	/*Step s;
	s.setDeltaTime(animation->time);
		
	animation->time = 0;
	animation->referenceValue = animation->startValue;
	animation->currentTime = 0;
	animation->currentTween = 0;

	animation->update(&s);*/
    animation->referenceValue = oldReferenceValue;
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