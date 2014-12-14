#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"
#include "Commands/CMD_EditTween.h"
#include "Commands/CMD_EditStartKey.h"
#include <cinder/app/AppBasic.h>

template<typename T>
class CMD_AddTweenInside : public Command{
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
	CMD_AddTweenInside(Animation<T> * _animation, float _deltaTimeline, T _targetValue, Easing::Type _interpolation, unsigned long int _nextTweenIndex);
	~CMD_AddTweenInside();

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

	unsigned long int nextTweenIndex;

	float nextTween_oldDeltaTime;
	T nextTween_oldDeltaValue;
	float nextTween_newDeltaTime;
	T nextTween_newDeltaValue;
};


template<typename T>
CMD_AddTweenInside<T>::CMD_AddTweenInside(Animation<T> * _animation, float _deltaTimeline, T _targetValue, Easing::Type _interpolation, unsigned long int _nextTweenIndex) :
	animation(_animation),
	deltaTimeline(_deltaTimeline),
	targetValue(_targetValue),
	tween(nullptr),
	interpolation(_interpolation),
	nextTweenIndex(_nextTweenIndex)
{	
}

template<typename T>
bool CMD_AddTweenInside<T>::execute(){
	ci::app::console() << "execute CMD_AddTweenInside" << std::endl;

	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;
		T deltaValue;

		// d2s for time and value are previous tween's time and value
		if(nextTweenIndex == 0){
			// Interrupting first tween
			deltaTime = targetTime;
			deltaValue = targetValue - animation->startValue;
		}else{
			// Interrupting a different tween
			deltaTime = targetTime - animation->getTweenEndTime(nextTweenIndex-1);
			deltaValue = targetValue - animation->getTweenEndValue(nextTweenIndex-1);
		}

		nextTween_oldDeltaTime = animation->tweens.at(nextTweenIndex)->deltaTime;
		nextTween_oldDeltaValue = animation->tweens.at(nextTweenIndex)->deltaValue;
		nextTween_newDeltaTime = animation->tweens.at(nextTweenIndex)->deltaTime - deltaTime;
		nextTween_newDeltaValue = animation->tweens.at(nextTweenIndex)->deltaValue - deltaValue;
		
		
		tween = new Tween<T>(deltaTime, deltaValue, interpolation);
	}

	// insert tween
	animation->tweens.insert(animation->tweens.begin() + nextTweenIndex, tween);

	animation->tweens.at(nextTweenIndex+1)->deltaTime = nextTween_newDeltaTime;
	animation->tweens.at(nextTweenIndex+1)->deltaValue = nextTween_newDeltaValue;

	return true;
}

template<>
bool CMD_AddTweenInside<glm::quat>::execute(){
	ci::app::console() << "execute CMD_AddTweenInside" << std::endl;

	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;
		glm::quat deltaValue;

		// d2s for time and value are previous tween's time and value
		if(nextTweenIndex == 0){
			// Interrupting first tween
			deltaTime = targetTime;
			deltaValue = glm::inverse(animation->startValue) * targetValue;
		}else{
			// Interrupting a different tween
			deltaTime = targetTime - animation->getTweenEndTime(nextTweenIndex-1);
			deltaValue = glm::inverse(animation->getTweenEndValue(nextTweenIndex-1)) * targetValue;
		}

		nextTween_oldDeltaTime = animation->tweens.at(nextTweenIndex)->deltaTime;
		nextTween_oldDeltaValue = animation->tweens.at(nextTweenIndex)->deltaValue;
		nextTween_newDeltaTime = animation->tweens.at(nextTweenIndex)->deltaTime - deltaTime;

		glm::quat end;
		end = nextTween_oldDeltaValue * animation->getTweenEndValue(nextTweenIndex);

		//nextTween_newDeltaValue = glm::inverse(deltaValue) * animation->tweens.at(nextTweenIndex)->deltaValue;
		
		nextTween_newDeltaValue = glm::inverse(targetValue) * end;

		tween = new Tween<glm::quat>(deltaTime, deltaValue, interpolation);
	}

	// insert tween
	animation->tweens.insert(animation->tweens.begin() + nextTweenIndex, tween);

	animation->tweens.at(nextTweenIndex+1)->deltaTime = nextTween_newDeltaTime;
	animation->tweens.at(nextTweenIndex+1)->deltaValue = nextTween_newDeltaValue;

	return true;
}

template<typename T>
bool CMD_AddTweenInside<T>::unexecute(){
	// Remove tween inside animation
	
	// after inserting, tween is in nextTweenIndex
	animation->tweens.erase(animation->tweens.begin() + nextTweenIndex);
	animation->tweens.at(nextTweenIndex)->deltaTime = nextTween_oldDeltaTime;
	animation->tweens.at(nextTweenIndex)->deltaValue = nextTween_oldDeltaValue;

	return true;
}

template<typename T>
CMD_AddTweenInside<T>::~CMD_AddTweenInside(){
	if(!executed){
		delete tween;
	}
	tween = nullptr;
}