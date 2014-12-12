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
	/* // Right now, this is only called when adding a tween before the animation, which already makes this tween with the correct deltaValue
	if(animation->tweens.size() > 0){
		animation->tweens.at(0)->deltaValue -= oldStartValue - targetValue;
	}
	*/
	animation->time -= targetTime;
	//animation->referenceValue = targetValue;
	return true;
}

bool CMD_EditStartKey::unexecute(){
	if(animation->tweens.size() > 0){
		animation->tweens.at(0)->deltaValue += oldStartValue - targetValue;
	}

	animation->startValue = oldStartValue;
	//animation->referenceValue = oldReferenceValue;
	animation->time += targetTime;
	return true;
}

CMD_EditStartKey::~CMD_EditStartKey(){
}