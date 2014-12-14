#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"
#include <cinder\app\App.h>

template<typename T>
class CMD_RemoveLastTween : public Command{
public:
	/**
	* Adds a tween to the specified animation
	*
	* @param _animation		The animation object to modify
	*/
	CMD_RemoveLastTween(Animation<T> * _animation);
	~CMD_RemoveLastTween();

	inline bool execute();
	bool unexecute();

private:
	// Used in getTweenEndValue
	Animation<T> * animation;
	
	Tween<T> * tween;
};


template<typename T>
CMD_RemoveLastTween<T>::CMD_RemoveLastTween(Animation<T> * _animation) :
	animation(_animation),
	tween(nullptr)
{	
}

template<typename T>
bool CMD_RemoveLastTween<T>::execute(){
	ci::app::console() << "execute CMD_RemoveLastTween" << std::endl;

	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){
		tween = animation->tweens.back();
	}
	
	// remove tween
	animation->tweens.pop_back();
	return true;
}

template<typename T>
bool CMD_RemoveLastTween<T>::unexecute(){
	// Remove tween after animation
	animation->tweens.push_back(tween);
	return true;
}

template<typename T>
CMD_RemoveLastTween<T>::~CMD_RemoveLastTween(){
	if(executed){
		delete tween;
	}
	tween = nullptr;
}