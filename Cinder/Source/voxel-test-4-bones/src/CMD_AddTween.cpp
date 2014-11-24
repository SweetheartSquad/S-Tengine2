#pragma once

#include "CMD_AddTween.h"
#include "CMD_EditTween.h"
#include "CMD_EditStartKey.h"

#include <algorithm>

#include <cinder/app/AppBasic.h>

CMD_AddTween::CMD_AddTween(Animation * _animation, float _currentTime, float _targetTime, float _targetValue, Easing::Type _interpolation) :
	animation(_animation),
	deltaTimeline(_targetTime - _currentTime),
	targetValue(_targetValue),
	tween(nullptr),
	interpolation(_interpolation),
	executed(false)
{	
}

void CMD_AddTween::execute(){
	ci::app::console() << "execute CMD_AddTween" << std::endl;
	
	// calculate values for new tween, and save other values that will be changed by this tween insert
	if (tween == nullptr){

		float targetTime = animation->time + deltaTimeline;
		float deltaTime;// = deltaTimeline;
		float deltaValue;// = targetValue;

		if(targetTime <= 0){
			// Before the animation

			deltaTime = 0 - targetTime;
			// d2s for time and value are 0 and start value (deltaTime stays as time)
			deltaValue = animation->startValue - targetValue;

			if(subCommands.size() == 0){
				subCommands.push_back(new CMD_EditStartKey(animation, targetValue));
			}
		}else{
			float sumTime = 0;
			bool insideAnimation = false;
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
				

			}else{
				// After the animation
				deltaTime = targetTime - sumTime;
				if(animation->tweens.size() > 0){
					deltaValue = targetValue - animation->getTweenEndValue(animation->tweens.size()-1);
				}else{
					deltaValue = targetValue - animation->startValue;
				}
			}
		}
		
		tween = new Tween(deltaTime, deltaValue, interpolation);
	}

	// Get insert iterator
	std::vector<Tween *>::iterator nextTween_it;
	if(nextTweenIndex < animation->tweens.size()){
		nextTween_it = animation->tweens.begin() + nextTweenIndex;
	}else{
		nextTween_it = animation->tweens.end();
	}
	
	// insert tween
	animation->tweens.insert(nextTween_it, tween);

	// update next tween's delta time and delta value, if it exists, can't use EditTween cmd because the next tween's index will change when we execute and unexecute inserting the new tween
	/*if(nextTweenIndex < animation->tweens.size()){
		if(subCommands.size() == 0){
			subCommands.push_back(new CMD_EditTween(animation, targetValue + nextTween_newDeltaValue, animation->tweens.at(nextTweenIndex+1)->interpolation, nextTweenIndex+1));
		}
	}*/
	if(nextTweenIndex < animation->tweens.size()){
		animation->tweens.at(nextTweenIndex+1)->deltaTime = nextTween_newDeltaTime;
		animation->tweens.at(nextTweenIndex+1)->deltaValue = nextTween_newDeltaValue;
	}

	if(subCommands.size() > 0){
		subCommands.at(0)->execute();
	}

	executed = true;
}

void CMD_AddTween::unexecute(){
	for(unsigned long int i = subCommands.size(); i > 0; --i){
		subCommands.at(i-1)->execute();
	}

	// Remove tween
	if(nextTweenIndex < animation->tweens.size()){
		// Inside animation

		// after inserting, tween is in nextTweenIndex
		animation->tweens.erase(animation->tweens.begin() + nextTweenIndex);
		animation->tweens.at(nextTweenIndex+1)->deltaTime = nextTween_oldDeltaTime;
		animation->tweens.at(nextTweenIndex+1)->deltaValue = nextTween_oldDeltaValue;

	}else{
		// Before/After animation
		animation->tweens.erase((animation->tweens.at(0) == tween) ? (animation->tweens.begin()) : (animation->tweens.begin() + animation->tweens.size() - 1));
	}

	executed = false;
}

CMD_AddTween::~CMD_AddTween(){
	if(!executed){
		delete tween;
	}
}