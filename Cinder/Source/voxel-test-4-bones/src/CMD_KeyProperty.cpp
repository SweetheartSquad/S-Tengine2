#pragma once

#include "CMD_KeyProperty.h"
#include "CMD_AddTween.h"
#include "CMD_EditTween.h"
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
	if(!animation->hasStart){
		// If there are no keyframes and the start hasn't been set
		oldStartValue = animation->startValue;

		// set start value
		animation->startValue = value;
		animation->referenceValue = value;
		animation->hasStart = true;
		oldHasStart = false;
	}else{
		// subtract the change in time from the animation's time, if there has been any change
		if((animation->time - (UI::time - currentTime)) + (targetTime - currentTime) == 0){
			// Edit start of the animation
			oldStartValue = animation->startValue;
			oldHasStart = true;

			// set start value
			animation->startValue = value;
			animation->referenceValue = value;

			if(animation->tweens.size() > 0){
				// If there are other keyframes
				animation->tweens.at(0)->deltaValue -= (value - oldStartValue);
			}
		}else{
			// Edit or add a tween
			if(firstRun){
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
			}else{
				subCmdProc.redo();
			}
		}
	}
	// We need to update the animation's time and reference value to the current time (not necessarily the same time as first run) using the new tweens/start value somehow
	Step * s = new Step();
	s->setDeltaTime(animation->time);
	ci::app::console() << "step deltaTime: " << animation->time << std::endl;
	animation->time = 0;
	animation->referenceValue = animation->startValue;
	animation->currentTime = 0;
	animation->currentTween = 0;
		
	animation->update(s);
}

void CMD_KeyProperty::unexecute(){

	// subtract the change in time from the animation's time, if there has been any change
	// since I'm adjusting the animation's time if this adds a new tween at the beginning, this won't work, since it's time at this frame wil be zero...
	if((animation->time - (UI::time - currentTime)) + (targetTime - currentTime) != 0){
		subCmdProc.undo();
	}else{
		animation->startValue = oldStartValue;
		animation->hasStart = oldHasStart;
		ci::app::console() << "CMD_KeyProperty unexecute" << std::endl;
		if(oldHasStart == true || animation->tweens.size() > 0){
			animation->tweens.at(0)->deltaValue += (value - oldStartValue);
		}
	}
	// We need to update the animation's time and reference value to the current time (not necessarily the same time as first run) using the restored tweens/start value somehow
	Step * s = new Step();
	s->setDeltaTime(animation->time);
		
	animation->time = 0;
	animation->referenceValue = animation->startValue;
	animation->currentTime = 0;
	animation->currentTween = 0;

	animation->update(s);
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