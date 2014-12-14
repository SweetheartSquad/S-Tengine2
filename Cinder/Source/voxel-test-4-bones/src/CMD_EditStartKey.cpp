#pragma once

#include "CMD_EditStartKey.h"
#include "CMD_EditTween.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_EditStartKey::CMD_EditStartKey(Animation * _animation, float _targetValue, float _targetTime) :
	animation(_animation),
	targetValue(_targetValue),
	targetTime(_targetTime)
{	
}

bool CMD_EditStartKey::execute(){
    oldHasStart = animation->hasStart;
	oldStartValue = animation->startValue;
	oldReferenceValue = animation->referenceValue;
	oldCurrentAnimationTime = animation->currentAnimationTime;
	animation->startValue = targetValue;
	if(animation->hasStart && targetTime == 0){
	    if(animation->tweens.size() > 0){
		    animation->tweens.at(0)->deltaValue += targetValue - oldStartValue;
	    }
	}else{
		if(animation->hasStart){
			animation->currentAnimationTime -= targetTime;
		}else{
			animation->currentAnimationTime = 0;
			animation->hasStart = true;
		}
	}
	return true;
}

bool CMD_EditStartKey::unexecute(){
    if(targetTime == 0){
	    if(animation->tweens.size() > 0){
		    animation->tweens.at(0)->deltaValue += oldStartValue - targetValue;
	    }
	}
    animation->hasStart = oldHasStart;
	animation->startValue = oldStartValue;
	animation->currentAnimationTime = oldCurrentAnimationTime;
	return true;
}

CMD_EditStartKey::~CMD_EditStartKey(){
}