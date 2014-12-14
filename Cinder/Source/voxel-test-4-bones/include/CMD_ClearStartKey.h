#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"

#include <cinder/app/AppBasic.h>


template<typename T>
class CMD_ClearStartKey : public Command {
public:
	/**
	* Edits the start of the specified animation
	*
	* @param _animation		The animation object to modify
	* @param _value			The target value at the start of the animation
	*/
	CMD_ClearStartKey(Animation<T> * _animation);
	~CMD_ClearStartKey();

	inline bool execute();
	bool unexecute();

private:
	Animation<T> * animation;
	
    bool oldHasStart;
};


template<typename T>
CMD_ClearStartKey<T>::CMD_ClearStartKey(Animation<T> * _animation) :
	animation(_animation)
{	
}

template<typename T>
bool CMD_ClearStartKey<T>::execute(){
    oldHasStart = animation->hasStart;
	animation->hasStart = false;

	return true;
}

template<typename T>
bool CMD_ClearStartKey<T>::unexecute(){
    animation->hasStart = oldHasStart;

	return true;
}

template<typename T>
CMD_ClearStartKey<T>::~CMD_ClearStartKey(){
}