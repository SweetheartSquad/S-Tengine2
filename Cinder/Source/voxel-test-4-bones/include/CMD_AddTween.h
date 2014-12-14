#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"
#include "CMD_AddTweenInside.h"
#include "CMD_AddTweenAfter.h"
#include "CMD_AddTweenBefore.h"

template<typename T>
class CMD_AddTween : public Command{
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
	CMD_AddTween(Animation<T> * _animation, float _currentTime, float _targetTime, T _targetValue, Easing::Type _interpolation);
	~CMD_AddTween();

	bool execute();
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

    unsigned long int oldCurrentTween, newCurrentTween;
	float	oldCurrentAnimationTime, newCurrentAnimationTime,
			oldCurrentTweenTime, newCurrentTweenTime;
	T		oldReferenceValue, newReferenceValue;
};

template<typename T>
CMD_AddTween<T>::CMD_AddTween(Animation<T> * _animation, float _currentTime, float _targetTime, T _targetValue, Easing::Type _interpolation) :
	animation(_animation),
	deltaTimeline(_targetTime - _currentTime),
	targetValue(_targetValue),
	interpolation(_interpolation),
    oldCurrentTween(-1),
	newCurrentTween(-1),
	oldCurrentAnimationTime(0),
	newCurrentAnimationTime(0),
	oldCurrentTweenTime(0),
	newCurrentTweenTime(0)
{	
}

template<typename T>
bool CMD_AddTween<T>::execute(){
	ci::app::console() << "execute CMD_AddTween" << std::endl;
    
    // calculate values for new tween, and save other values that will be changed by this tween insert
	if (firstRun){
		/*oldCurrentAnimationTime = animation->currentAnimationTime;
		oldCurrentTweenTime = animation->currentTweenTime;
		oldCurrentTween = animation->currentTween;
		oldReferenceValue = animation->referenceValue;*/

		float targetTime = animation->currentAnimationTime + deltaTimeline;

		if(targetTime <= 0){
            newCurrentTween = 0;
			subCmdProc.executeCommand(new CMD_AddTweenBefore<T>(animation, deltaTimeline, targetValue, interpolation));
		}else{
			float sumTime = 0;
			bool insideAnimation = false;
			unsigned long int nextTweenIndex = -1;
			for(unsigned long int i = 0; i < animation->tweens.size(); ++i){
				sumTime += animation->tweens.at(i)->deltaTime;
				if(sumTime > targetTime){
					// Inside the animation
					insideAnimation = true;
					nextTweenIndex = i;
					break;
				}
			}

			if(insideAnimation){
               // newCurrentTween = nextTweenIndex;
				subCmdProc.executeCommand(new CMD_AddTweenInside<T>(animation, deltaTimeline, targetValue, interpolation, nextTweenIndex));
			}else{
				// After the animation
                //newCurrentTween = animation->tweens.size();
				subCmdProc.executeCommand(new CMD_AddTweenAfter<T>(animation, deltaTimeline, targetValue, interpolation, sumTime));
			}
		}
		Step s;
		s.setDeltaTime(animation->currentAnimationTime);
		animation->currentAnimationTime = 0;
		animation->currentTweenTime = 0;
		animation->currentTween = 0;
		animation->referenceValue = animation->startValue;
		animation->update(&s);

		// save vals
		newCurrentAnimationTime = animation->currentAnimationTime;
		newCurrentTweenTime = animation->currentTweenTime;
		newCurrentTween = animation->currentTween;
		newReferenceValue = animation->referenceValue;
	}else{
		subCmdProc.redo();
		
		animation->currentAnimationTime = newCurrentAnimationTime;
		animation->currentTweenTime = newCurrentTweenTime;
		animation->currentTween = newCurrentTween;
		animation->referenceValue = newReferenceValue;
	}

	return true;
}

template<typename T>
bool CMD_AddTween<T>::unexecute(){
	
	animation->currentAnimationTime = oldCurrentAnimationTime;
	animation->currentTweenTime = oldCurrentTweenTime;
	animation->currentTween = oldCurrentTween;
	animation->referenceValue = oldReferenceValue;

    subCmdProc.undo();
	return true;
}

template<typename T>
CMD_AddTween<T>::~CMD_AddTween(){
}