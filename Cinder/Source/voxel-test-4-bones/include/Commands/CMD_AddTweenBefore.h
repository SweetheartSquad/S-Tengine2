#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"
#include "Commands/CMD_EditStartKey.h"
#include <glm\glm.hpp>
#include <cinder\app\App.h>

template<typename T>
class CMD_AddTweenBefore : public Command{
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
	CMD_AddTweenBefore(Animation<T> * _animation, float _deltaTimeline, T _targetValue, Easing::Type _interpolation);
	~CMD_AddTweenBefore();

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
};

template<typename T>
CMD_AddTweenBefore<T>::CMD_AddTweenBefore(Animation<T> * _animation, float _deltaTimeline, T _targetValue, Easing::Type _interpolation) :
	animation(_animation),
	deltaTimeline(_deltaTimeline),
	targetValue(_targetValue),
	tween(nullptr),
	interpolation(_interpolation)
{	
}

template<typename T>
bool CMD_AddTweenBefore<T>::execute(){
	ci::app::console() << "execute CMD_AddTweenBefore" << std::endl;
	
	CMD_EditStartKey<T> * cmd;
	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;// = deltaTimeline;
		T deltaValue;// = targetValue;

		deltaTime = 0 - targetTime;
		// d2s for time and value are 0 and start value (deltaTime stays as time)
		deltaValue = animation->startValue - targetValue;

		cmd = new CMD_EditStartKey<T>(animation, targetValue, targetTime);
		tween = new Tween<T>(deltaTime, deltaValue, interpolation);
	}

	// insert tween
	animation->tweens.insert(animation->tweens.begin(), tween);

	if(firstRun){
		subCmdProc.executeCommand(cmd);
	}else{
		subCmdProc.redo();
	}

	return true;
}
template<>
bool CMD_AddTweenBefore<glm::quat>::execute(){
	ci::app::console() << "execute CMD_AddTweenBefore" << std::endl;
	
	CMD_EditStartKey<glm::quat> * cmd;
	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){

		float targetTime = animation->currentAnimationTime + deltaTimeline;
		float deltaTime;// = deltaTimeline;
		glm::quat deltaValue;// = targetValue;

		deltaTime = 0 - targetTime;
		// d2s for time and value are 0 and start value (deltaTime stays as time)
		deltaValue = glm::inverse(targetValue) * animation->startValue;

		cmd = new CMD_EditStartKey<glm::quat>(animation, targetValue, targetTime);
		tween = new Tween<glm::quat>(deltaTime, deltaValue, interpolation);
	}

	// insert tween
	animation->tweens.insert(animation->tweens.begin(), tween);

	if(firstRun){
		subCmdProc.executeCommand(cmd);
	}else{
		subCmdProc.redo();
	}

	return true;
}

template<typename T>
bool CMD_AddTweenBefore<T>::unexecute(){
    subCmdProc.undo();
	// Remove tween before animation
	animation->tweens.erase(animation->tweens.begin());
	return true;
}

template<typename T>
CMD_AddTweenBefore<T>::~CMD_AddTweenBefore(){
	if(!executed){
		delete tween;
	}
	tween = nullptr;
}