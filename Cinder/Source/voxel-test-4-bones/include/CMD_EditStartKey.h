#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"

#include <cinder/app/AppBasic.h>


template<typename T>
class CMD_EditStartKey : public Command {
public:
	/**
	* Edits the start of the specified animation
	*
	* @param _animation		The animation object to modify
	* @param _value			The target value at the start of the animation
	*/
	CMD_EditStartKey(Animation<T> * _animation, T _targetValue, float _targetTime);
	~CMD_EditStartKey();

	inline bool execute();
	bool unexecute();

private:
	Animation<T> * animation;
	T targetValue;
	float targetTime;
	
	T oldDeltaValue;
	T oldStartValue;
	T oldReferenceValue;
    bool oldHasStart;
	float oldCurrentAnimationTime;
};


template<typename T>
CMD_EditStartKey<T>::CMD_EditStartKey(Animation<T> * _animation, T _targetValue, float _targetTime) :
	animation(_animation),
	targetValue(_targetValue),
	targetTime(_targetTime)
{	
}

template<typename T>
bool CMD_EditStartKey<T>::execute(){
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
template<>
bool CMD_EditStartKey<glm::quat>::execute(){
    oldHasStart = animation->hasStart;
	oldStartValue = animation->startValue;
	oldReferenceValue = animation->referenceValue;
	oldCurrentAnimationTime = animation->currentAnimationTime;
	animation->startValue = targetValue;
	if(animation->hasStart && targetTime == 0){
	    if(animation->tweens.size() > 0){
			oldDeltaValue = animation->tweens.at(0)->deltaValue;
			animation->tweens.at(0)->deltaValue = (glm::inverse(oldStartValue) * targetValue) * animation->tweens.at(0)->deltaValue;
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

template<typename T>
bool CMD_EditStartKey<T>::unexecute(){
    if(targetTime == 0){
	    if(animation->tweens.size() > 0){
		    animation->tweens.at(0)->deltaValue = oldDeltaValue;
	    }
	}
    animation->hasStart = oldHasStart;
	animation->startValue = oldStartValue;
	animation->currentAnimationTime = oldCurrentAnimationTime;
	return true;
}

template<typename T>
CMD_EditStartKey<T>::~CMD_EditStartKey(){
}