#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"
#include "Commands/CMD_EditTween.h"
#include "Commands/CMD_EditStartKey.h"
#include <cinder/app/AppBasic.h>

template<typename T>
class CMD_RemoveInnerTween : public Command{
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
	CMD_RemoveInnerTween(Animation<T> * _animation, unsigned long int _index);
	~CMD_RemoveInnerTween();

	inline bool execute();
	bool unexecute();

private:
	// Used in getTweenEndValue
	Animation<T> * animation;

	Tween<T> * tween;

	unsigned long int index;

	float nextTween_oldDeltaTime;
	T nextTween_oldDeltaValue;
	float nextTween_newDeltaTime;
	T nextTween_newDeltaValue;
};


template<typename T>
CMD_RemoveInnerTween<T>::CMD_RemoveInnerTween(Animation<T> * _animation, unsigned long int _index) :
	animation(_animation),
	tween(nullptr),
	index(_index)
{	
}

template<typename T>
bool CMD_RemoveInnerTween<T>::execute(){
	ci::app::console() << "execute CMD_RemoveInnerTween" << std::endl;

	// calculate values for next tween, and save other values that will be changed by this tween removal
	if (firstRun){
		tween = animation->tweens.at(index);

		nextTween_oldDeltaTime = animation->tweens.at(index + 1)->deltaTime;
		nextTween_oldDeltaValue = animation->tweens.at(index + 1)->deltaValue;

		nextTween_newDeltaTime = tween->deltaTime + animation->tweens.at(index + 1)->deltaTime;
		nextTween_newDeltaValue = tween->deltaValue + animation->tweens.at(index + 1)->deltaValue;
	}

	// remove tween
	animation->tweens.erase(animation->tweens.begin() + index);

	animation->tweens.at(index)->deltaTime = nextTween_newDeltaTime;
	animation->tweens.at(index)->deltaValue = nextTween_newDeltaValue;

	return true;
}

template<>
bool CMD_RemoveInnerTween<glm::quat>::execute(){
	ci::app::console() << "execute CMD_RemoveInnerTween" << std::endl;

	// calculate values for next tween, and save other values that will be changed by this tween removal
	if (firstRun){
		tween = animation->tweens.at(index);

		nextTween_oldDeltaTime = animation->tweens.at(index + 1)->deltaTime;
		nextTween_oldDeltaValue = animation->tweens.at(index + 1)->deltaValue;

		nextTween_newDeltaTime = tween->deltaTime + animation->tweens.at(index + 1)->deltaTime;
		nextTween_newDeltaValue = animation->tweens.at(index + 1)->deltaValue * tween->deltaValue;
	}

	// remove tween
	animation->tweens.erase(animation->tweens.begin() + index);

	animation->tweens.at(index)->deltaTime = nextTween_newDeltaTime;
	animation->tweens.at(index)->deltaValue = nextTween_newDeltaValue;

	return true;
}

template<typename T>
bool CMD_RemoveInnerTween<T>::unexecute(){
	// Restore tween inside animation
	
	// after removing, nextTween is in index
	animation->tweens.insert(animation->tweens.begin() + index, tween);
	animation->tweens.at(index + 1)->deltaTime = nextTween_oldDeltaTime;
	animation->tweens.at(index + 1)->deltaValue = nextTween_oldDeltaValue;

	return true;
}

template<typename T>
CMD_RemoveInnerTween<T>::~CMD_RemoveInnerTween(){
	if(executed){
		delete tween;
	}
	tween = nullptr;
}