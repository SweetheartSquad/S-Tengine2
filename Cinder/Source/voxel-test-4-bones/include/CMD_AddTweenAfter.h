#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"
#include <cinder\app\App.h>

template<typename T>
class CMD_AddTweenAfter : public Command{
public:
	/**
	* Adds a tween to the specified animation
	*
	* @param _animation		The animation object to modify
	* @param _currentTime	The current time in the scene
	* @param _targetTime	The target time at the end of the added tween
	* @param _value			The target value at the end of the added tween
	* @param _interpolation	The interpolation type for the tween
	*/
	CMD_AddTweenAfter(Animation<T> * _animation, float _deltaTimeline, T _targetValue, Easing::Type _interpolation, float _sumTime);
	~CMD_AddTweenAfter();

	inline bool execute();
	bool unexecute();

private:
	// Used in getTweenEndValue
	Animation<T> * animation;

	// Time between the current time on the timeline and the target end-time for the tween
	float deltaTimeline;
	// Value of the property at the given time
	T targetValue;
	// Interpolation type for the tween
	Easing::Type interpolation;

	Tween<T> * tween;

	float sumTime;
};


template<typename T>
CMD_AddTweenAfter<T>::CMD_AddTweenAfter(Animation<T> * _animation, float _deltaTimeline, T _targetValue, Easing::Type _interpolation, float _sumTime) :
	animation(_animation),
	deltaTimeline(_deltaTimeline),
	targetValue(_targetValue),
	tween(nullptr),
	interpolation(_interpolation),
    sumTime(_sumTime)
{	
}

template<typename T>
bool CMD_AddTweenAfter<T>::execute(){
	ci::app::console() << "execute CMD_AddTweenAfter" << std::endl;

	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;// = deltaTimeline;
		T deltaValue;// = targetValue;

		// After the animation
		deltaTime = targetTime - sumTime;
		if(animation->tweens.size() > 0){
			deltaValue = targetValue - animation->getTweenEndValue(animation->tweens.size()-1);
		}else{
			deltaValue = targetValue - animation->startValue;
		}
		
		tween = new Tween<T>(deltaTime, deltaValue, interpolation);
	}
	
	// insert tween
	animation->tweens.insert(animation->tweens.end(), tween);
	return true;
}

template<>
bool CMD_AddTweenAfter<glm::quat>::execute(){
	ci::app::console() << "execute CMD_AddTweenAfter" << std::endl;

	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;// = deltaTimeline;
		glm::quat deltaValue;// = targetValue;

		// After the animation
		deltaTime = targetTime - sumTime;
		if(animation->tweens.size() > 0){
			deltaValue = glm::inverse(animation->getTweenEndValue(animation->tweens.size()-1)) * targetValue;
		}else{
			deltaValue = glm::inverse(animation->startValue) * targetValue;
		}
		
		tween = new Tween<glm::quat>(deltaTime, deltaValue, interpolation);
	}
	
	// insert tween
	animation->tweens.insert(animation->tweens.end(), tween);
	return true;
}

template<typename T>
bool CMD_AddTweenAfter<T>::unexecute(){
	// Remove tween after animation
	animation->tweens.erase(animation->tweens.begin() + animation->tweens.size() - 1);
	return true;
}

template<typename T>
CMD_AddTweenAfter<T>::~CMD_AddTweenAfter(){
	if(!executed){
		delete tween;
	}
	tween = nullptr;
}