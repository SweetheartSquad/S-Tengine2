#pragma once

#include "Command.h"
#include "Easing.h"
#include <vector>
#include "Animation.h"
#include "CMD_EditStartKey.h"
#include "CMD_AddTween.h"
#include "CMD_EditTween.h"

template <typename T>
class CMD_KeyProperty : public Command{
public:
	/**
	* Adds or edits a keyframe for the specified property
	*
	* @param _animation		The animation object to modify
	* @param _currentTime	The current time in the scene
	* @param _targetTime	The target time to key
	* @param _value			The target value to key
	* @param _interpolation	The interpolation type to key
	*/
	inline CMD_KeyProperty(Animation<T> * _animation, float _currentTime, float _targetTime, T _value, Easing::Type _interpolation);
	
	~CMD_KeyProperty();

	bool execute();
	bool unexecute();

private:

	Animation<T> * animation;
	float currentTime;
	float targetTime;
	T value;
	Easing::Type interpolation;

	int findKeyframe(std::vector<Tween<T> *> * _tweens);

	T getStartValue(unsigned long int _idx);
	T getEndValue(unsigned long int _idx);
};

/*template class CMD_KeyProperty<float>;

template<>
CMD_KeyProperty<float>::CMD_KeyProperty(Animation<float> * _animation, float _currentTime, float _targetTime, float _value, Easing::Type _interpolation){};*/

template <typename T>
CMD_KeyProperty<T>::CMD_KeyProperty(Animation<T> * _animation, float _currentTime, float _targetTime, T _value, Easing::Type _interpolation) :
	animation(_animation),
	currentTime(_currentTime),
	targetTime(_targetTime),
	value(_value),
	interpolation(_interpolation)
{
}

template<typename T>
bool CMD_KeyProperty<T>::execute(){
	// Executing for the first time, save the oldStartValue if keying 0, or create an add or edit command
	if(firstRun){
	    if(!animation->hasStart){
		    // If there are no keyframes and the start hasn't been set
            subCmdProc.executeCommand(new CMD_EditStartKey<T>(animation, value, targetTime));
	    }else{
		    // subtract the change in time from the animation's time, if there has been any change
		    if((animation->currentAnimationTime - (UI::time - currentTime)) + (targetTime - currentTime) == 0){
			    subCmdProc.executeCommand(new CMD_EditStartKey<T>(animation, value, targetTime));
		    }else{
			    // Edit or add a tween
			
				// find index of tween
				int idx = -1;
				float sumTime = animation->currentAnimationTime;
				if(sumTime + (targetTime - currentTime) > 0){
					for(unsigned long int i = 0; i < animation->tweens.size(); ++i){
						sumTime -= animation->tweens.at(i)->deltaTime;
						if(sumTime == (targetTime - currentTime)){
							idx = i;
							break;
						}
					}
				}
				ci::app::console() << "sumTime: " << sumTime << std::endl;
				if(idx >= 0){
					subCmdProc.executeCommand(new CMD_EditTween<T>(animation, value, interpolation, idx));
				}else{
					subCmdProc.executeCommand(new CMD_AddTween<T>(animation, currentTime, targetTime, value, interpolation));
				}
		    }
        }
	}else{
		subCmdProc.redo();
	}

	return true;
}

template<typename T>
bool CMD_KeyProperty<T>::unexecute(){
	subCmdProc.undo();

	return true;
}

template<typename T>
T CMD_KeyProperty<T>::getStartValue(unsigned long int _idx){
	
	T value = animation->startValue;

	for(unsigned long int i = 0; i < _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

template<typename T>
T CMD_KeyProperty<T>::getEndValue(unsigned long int _idx){
	T value = animation->startValue;

	for(unsigned long int i = 0; i = _idx; ++i){
		value += animation->tweens.at(i)->deltaValue;
	}

	return value;
}

template<typename T>
CMD_KeyProperty<T>::~CMD_KeyProperty(){
}