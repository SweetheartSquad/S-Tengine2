#pragma once

#include "Command.h"
#include "Easing.h"
#include <vector>
#include "Animation.h"

#include "Commands/CMD_ClearStartKey.h"
#include "Commands/CMD_RemoveFirstTween.h"
#include "Commands/CMD_RemoveInnerTween.h"
#include "Commands/CMD_RemoveLastTween.h"

template <typename T>
class CMD_ClearProperty : public Command{
public:
	/**
	* Adds or edits a keyframe for the specified property
	*
	* @param _animation		The animation object to modify
	* @param _currentTime	The current time in the scene
	* @param _targetTime	The target time to key
	*/
	inline CMD_ClearProperty(Animation<T> * _animation, float _currentTime, float _targetTime);
	
	~CMD_ClearProperty();

	bool execute();
	bool unexecute();

private:

	Animation<T> * animation;
	float currentTime;
	float targetTime;

	Tween<T> * tween;

	unsigned long int oldCurrentTween, newCurrentTween;
	float	oldCurrentAnimationTime, newCurrentAnimationTime,
			oldCurrentTweenTime, newCurrentTweenTime;
	T		oldReferenceValue, newReferenceValue, oldProperty;

	int findKeyframe(std::vector<Tween<T> *> * _tweens);
};

template <typename T>
CMD_ClearProperty<T>::CMD_ClearProperty(Animation<T> * _animation, float _currentTime, float _targetTime) :
	animation(_animation),
	currentTime(_currentTime),
	targetTime(_targetTime),
	tween(nullptr),
	oldCurrentTween(-1),
	newCurrentTween(-1),
	oldCurrentAnimationTime(0),
	newCurrentAnimationTime(0),
	oldCurrentTweenTime(0),
	newCurrentTweenTime(0)
{
}

template<typename T>
bool CMD_ClearProperty<T>::execute(){
	// Executing for the first time, save the oldStartValue if keying 0, or create an add or edit command
	if(firstRun){

	    if(!animation->hasStart){
		    // Shouldn't happen? No keyframe? Do nothing?
            // warn
	    }else{
		    // subtract the change in time from the animation's time, if there has been any change
		    if((animation->currentAnimationTime - (UI::time - currentTime)) + (targetTime - currentTime) == 0){
				// If we are at the starting frame
			    if(animation->tweens.size() > 0){
					// Remove first tween
					subCmdProc.executeCommand(new CMD_RemoveFirstTween<T>(animation, currentTime, targetTime));
				}else{
					// Remove start key
					subCmdProc.executeCommand(new CMD_ClearStartKey<T>(animation));
				}
		    }else{
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
					if(idx == animation->tweens.size() - 1){
						// remove a tween at the end of the animation
						subCmdProc.executeCommand(new CMD_RemoveLastTween<T>(animation));
					}else{
						// remove a tween inside the animation
						subCmdProc.executeCommand(new CMD_RemoveInnerTween<T>(animation, idx));
					}
				}else{
					// outside of the animation
					// warn
				}
		    }
        }
		
		oldCurrentAnimationTime = animation->currentAnimationTime;
		oldCurrentTweenTime = animation->currentTweenTime;
		oldCurrentTween = animation->currentTween;
		oldReferenceValue = animation->referenceValue;
		oldProperty = *animation->prop;
		Step s;
		s.setDeltaTime(animation->currentAnimationTime);
		animation->currentAnimationTime = 0;
		animation->currentTweenTime = 0;
		animation->currentTween = 0;
		animation->referenceValue = animation->startValue;
		animation->update(&s);

	}else{
		subCmdProc.redo();
		
		Step s;
		s.setDeltaTime(animation->currentAnimationTime);
		animation->currentAnimationTime = 0;
		animation->currentTweenTime = 0;
		animation->currentTween = 0;
		animation->referenceValue = animation->startValue;
		animation->update(&s);
	}

	return true;
}

template<typename T>
bool CMD_ClearProperty<T>::unexecute(){

	Step s;
	s.setDeltaTime(oldCurrentAnimationTime);
	animation->currentAnimationTime = 0;
	animation->currentTweenTime = 0;
	animation->currentTween = 0;
	animation->referenceValue = animation->startValue;
	animation->update(&s);

	*(animation->prop) = oldProperty;

	subCmdProc.undo();
	
	return true;
}

template<typename T>
CMD_ClearProperty<T>::~CMD_ClearProperty(){
}