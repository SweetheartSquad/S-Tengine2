#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"
#include "CMD_EditStartKey.h"
#include <glm\glm.hpp>
#include <cinder\app\App.h>

template<typename T>
class CMD_RemoveFirstTween : public Command{
public:
	/**
	* Adds a tween to the specified animation
	*
	* @param _animation		The animation object to modify
	*/
	CMD_RemoveFirstTween(Animation<T> * _animation, float _currentTime, float _targetTime);
	~CMD_RemoveFirstTween();

	inline bool execute();
	bool unexecute();

private:
	// Used in getTweenEndValue
	Animation<T> * animation;

	// Time between the current time on the timeline and the target end-time for the tween
	float deltaTimeline;

	Tween<T> * tween;
};

template<typename T>
CMD_RemoveFirstTween<T>::CMD_RemoveFirstTween(Animation<T> * _animation, float _currentTime, float _targetTime) :
	animation(_animation),
	deltaTimeline(_targetTime - _currentTime),
	tween(nullptr)
{	
}

template<typename T>
bool CMD_RemoveFirstTween<T>::execute(){
	ci::app::console() << "execute CMD_RemoveFirstTween" << std::endl;
	
	CMD_EditStartKey<T> * cmd;
	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){
		tween = animation->tweens.front();
		float targetTime = animation->currentAnimationTime + deltaTimeline;
		cmd = new CMD_EditStartKey<T>(animation, animation->startValue+tween->deltaValue, tween->deltaTime);
	}
	
	if(firstRun){
		subCmdProc.executeCommand(cmd);
	}else{
		subCmdProc.redo();
	}

	// remove tween
	animation->tweens.erase(animation->tweens.begin());

	return true;
}

template<>
bool CMD_RemoveFirstTween<glm::quat>::execute(){
	ci::app::console() << "execute CMD_RemoveFirstTween" << std::endl;
	
	CMD_EditStartKey<glm::quat> * cmd;
	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){
		tween = animation->tweens.front();
		float targetTime = animation->currentAnimationTime + deltaTimeline;
		cmd = new CMD_EditStartKey<glm::quat>(animation, glm::inverse(tween->deltaValue) * animation->startValue, tween->deltaTime);
	}

	// remove tween
	animation->tweens.erase(animation->tweens.begin());
	
	if(firstRun){
		subCmdProc.executeCommand(cmd);
	}else{
		subCmdProc.redo();
	}

	return true;
}


template<typename T>
bool CMD_RemoveFirstTween<T>::unexecute(){
	// Restore tween before animation
	animation->tweens.insert(animation->tweens.begin(), tween);

	subCmdProc.undo();
	return true;
}

template<typename T>
CMD_RemoveFirstTween<T>::~CMD_RemoveFirstTween(){
	if(executed){
		delete tween;
	}
	tween = nullptr;
}