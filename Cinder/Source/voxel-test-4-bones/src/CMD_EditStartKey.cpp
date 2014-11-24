#pragma once

#include "CMD_EditStartKey.h"
#include "CMD_EditTween.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_EditStartKey::CMD_EditStartKey(Animation * _animation, float _targetValue) :
	animation(_animation),
	targetValue(_targetValue)
{	
}

void CMD_EditStartKey::execute(){
	oldStartValue = animation->startValue;
	animation->startValue = targetValue;
	if(animation->tweens.size() > 0){
		animation->tweens.at(0)->deltaValue -= oldStartValue - targetValue;
	}
}

void CMD_EditStartKey::unexecute(){
	if(animation->tweens.size() > 0){
		animation->tweens.at(0)->deltaValue += oldStartValue - targetValue;
	}

	animation->startValue = oldStartValue;
}

CMD_EditStartKey::~CMD_EditStartKey(){
}