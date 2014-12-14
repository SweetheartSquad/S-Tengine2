#pragma once

#include "Command.h"
#include "Easing.h"
#include "Tween.h"
#include "Animation.h"
#include <cinder/app/App.h>

template<typename T>
class CMD_EditTween : public Command{
public:
	CMD_EditTween(Animation<T> * _animation, T _targetValue, Easing::Type _interpolation, int _idx);
	~CMD_EditTween();

	inline bool execute();
	bool unexecute();

private:
	// Used in getTweenEndValue function
	Animation<T> * animation;

	// New value of the property at the given time, used to calculate new deltaValue
	T targetValue;
	// New interpolation type for the tween
	Easing::Type interpolation;

	// New deltaValue to be set in the tween
	T newDeltaValue;
	// Old deltaValue set in the tween
	T oldDeltaValue;
	// Old interpolation type of the tween
	Easing::Type oldInterpolation;

	// The index of the tween
	int idx;
	Tween<T> * tween;

	// Tween after this tween in animation, will be updated if this tween's deltaValue changes
	unsigned long int nextTweenIndex; // -1 if none
	T nextTween_oldDeltaValue;
	T nextTween_newDeltaValue;
};

template<typename T>
CMD_EditTween<T>::CMD_EditTween(Animation<T> * _animation, T _targetValue, Easing::Type _interpolation, int _idx) :
	animation(_animation),
	targetValue(_targetValue),
	tween(_animation->tweens.at(_idx)),
	idx(_idx),
	interpolation(_interpolation)
{
}

template<typename T>
bool CMD_EditTween<T>::execute(){
	ci::app::console() << "execute CMD_EditTween" << std::endl;
	// save old tween values and calculate other values being affected by this tween edit
	if (!executed){
		// Save old deltaValue of edited tween
		oldDeltaValue = tween->deltaValue;

		// Get index of next tween, if it exists, else -1
		nextTweenIndex = idx == animation->tweens.size()-1 ? -1 : idx + 1;
		if(nextTweenIndex < animation->tweens.size()){
			nextTween_oldDeltaValue = animation->tweens.at(nextTweenIndex)->deltaValue;
			nextTween_newDeltaValue = nextTween_oldDeltaValue + (newDeltaValue - oldDeltaValue);
		}

		// Calculate deltaValue of edited tween
		if(idx > 0){
			// d2 for value is previous tween's value
			newDeltaValue = targetValue - animation->getTweenEndValue(idx-1);
		}else{
			// d2 for value is start value
			newDeltaValue = targetValue - animation->startValue;
		}
	}
	
	// Edit tween values
	tween->deltaValue = newDeltaValue;
	tween->interpolation = interpolation;

	// Change next keyframe's start value, if this one's is changing
	if (newDeltaValue != oldDeltaValue){
		if (nextTweenIndex < animation->tweens.size()){
			animation->tweens.at(nextTweenIndex)->deltaValue = nextTween_newDeltaValue;
		}
	}
	return true;
}

template<>
bool CMD_EditTween<glm::quat>::execute(){
	ci::app::console() << "execute CMD_EditTween" << std::endl;
	// save old tween values and calculate other values being affected by this tween edit
	if (!executed){
		// Save old deltaValue of edited tween
		oldDeltaValue = tween->deltaValue;

		// Get index of next tween, if it exists, else -1
		nextTweenIndex = idx == animation->tweens.size()-1 ? -1 : idx + 1;
		if(nextTweenIndex < animation->tweens.size()){
			nextTween_oldDeltaValue = animation->tweens.at(nextTweenIndex)->deltaValue;
			nextTween_newDeltaValue = (glm::inverse(newDeltaValue) * oldDeltaValue) * nextTween_oldDeltaValue;
		}

		// Calculate deltaValue of edited tween
		if(idx > 0){
			// d2 for value is previous tween's value
			newDeltaValue = glm::inverse(targetValue) * animation->getTweenEndValue(idx-1);
		}else{
			// d2 for value is start value
			newDeltaValue = glm::inverse(targetValue) * animation->startValue;
		}
	}
	
	// Edit tween values
	tween->deltaValue = newDeltaValue;
	tween->interpolation = interpolation;

	// Change next keyframe's start value, if this one's is changing
	if (newDeltaValue != oldDeltaValue){
		if (nextTweenIndex < animation->tweens.size()){
			animation->tweens.at(nextTweenIndex)->deltaValue = nextTween_newDeltaValue;
		}
	}
	return true;
}


template<typename T>
bool CMD_EditTween<T>::unexecute(){
	tween->deltaValue = oldDeltaValue;
	tween->interpolation = oldInterpolation;

	// Restore next keyframe's start value, if this one's is changing
	if (newDeltaValue != oldDeltaValue){
		if (nextTweenIndex < animation->tweens.size()){
			animation->tweens.at(nextTweenIndex)->deltaValue = nextTween_oldDeltaValue;
		}
	}
	return true;
}

template<typename T>
CMD_EditTween<T>::~CMD_EditTween()
{
}