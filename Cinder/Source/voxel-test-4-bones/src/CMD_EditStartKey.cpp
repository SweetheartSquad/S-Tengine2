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
	oldStartValue = animation->startValue;
	oldReferenceValue = animation->referenceValue;
	animation->startValue = targetValue;
	//  // Right now, this is only called when adding a tween before the animation, which already makes this tween with the correct deltaValue
	if(animation->hasStart && targetTime == 0){
	    if(animation->tweens.size() > 0){
		    animation->tweens.at(0)->deltaValue += targetValue - oldStartValue; // - targetValue;
	    }
	}else{
        oldHasStart = animation->hasStart;
		if(animation->hasStart){
			animation->currentAnimationTime -= targetTime;
		}else{
			animation->currentAnimationTime = 0;
			animation->hasStart = true;
		}
	}
	//animation->referenceValue = targetValue;
	return true;
}

bool CMD_EditStartKey::unexecute(){
    if(targetTime == 0){
	    if(animation->tweens.size() > 0){
		    animation->tweens.at(0)->deltaValue += oldStartValue - targetValue;
	    }
	}else{
        animation->currentAnimationTime += targetTime;
        animation->hasStart = oldHasStart;
	}
	animation->startValue = oldStartValue;
	//animation->referenceValue = oldReferenceValue;

	animation->currentAnimationTime += targetTime;
	return true;
}

CMD_EditStartKey::~CMD_EditStartKey(){
}