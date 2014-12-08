#pragma once

#include "Animation.h"
#include "Step.h"

float Animation::getTweenEndTime(unsigned long int _idx){
	if(_idx < tweens.size()){
		float time = 0;
		for(unsigned long int i = 0; i <= _idx; ++i){
			time += tweens.at(i)->deltaTime;
		}
		return time;
	}else{
		throw "index outside bounds of tween array";
	}
}

float Animation::getTweenEndValue(unsigned long int _idx){
	if(_idx < tweens.size()){
		float value = startValue;
		for(unsigned long int i = 0; i <= _idx; ++i){
			value += tweens.at(i)->deltaValue;
		}
		return value;
	}else{
		throw "index outside bounds of tween array";
	}
}


Animation::Animation(float * const _prop) :
	prop(_prop),
	time(0),
	currentTime(0),
	currentTween(0),
	loopType(LoopType::LOOP),
	referenceValue(0),
	startValue(*_prop),
	hasStart(false)
{
}

void Animation::update(Step * _step){
	if(hasStart){
		time += (float)_step->getDeltaTime();
	}
	if(tweens.size() > 0){
		currentTime += (float)_step->getDeltaTime();
		if(_step->getReverse()){
			while(currentTime <= 0){
				currentTime += tweens.at(currentTween)->deltaTime;

				if(currentTween > 0){
					referenceValue -= tweens.at(currentTween-1)->deltaValue;
				}

				
				if(currentTween == 0){
					switch (loopType){
						case Animation::LOOP:
						case Animation::CONSTANT:
							referenceValue = startValue;
							for(unsigned long int i = 0; i+1 < tweens.size(); ++i){
								referenceValue += tweens.at(i)->deltaValue;
							}
						case Animation::LOOP_WITH_OFFSET:
							currentTween = tweens.size() - 1;
							break;
						default:
							break;
					}
				}else{
					--currentTween;
				}
			}
		}else{
			while(currentTime > tweens.at(currentTween)->deltaTime){
				currentTime -= tweens.at(currentTween)->deltaTime;
				referenceValue += tweens.at(currentTween)->deltaValue;
				++currentTween;

				if(currentTween >= tweens.size()){
					switch (loopType){
						case Animation::LOOP:
						case Animation::CONSTANT:
							referenceValue = startValue;
						case Animation::LOOP_WITH_OFFSET:
							currentTween = 0;
							break;
						default:
							break;
					}
				}
			}
		}
		
		*prop = Easing::call(tweens.at(currentTween)->interpolation, currentTime, referenceValue, tweens.at(currentTween)->deltaValue, tweens.at(currentTween)->deltaTime);
		
		if(loopType == CONSTANT){
			if(time < 0){
				*prop = startValue;
			}else if(time > getTweenEndTime(tweens.size()-1)){
				*prop = getTweenEndValue(tweens.size()-1);
			}
		}

		// Display the start-frame value on the start frame (ordinarily it would show the last frame-value if looping)
		if(time == 0){
			*prop = startValue;
		}
	}
}

Animation::~Animation(){
	while(tweens.size() > 0){
		delete tweens.back();
		tweens.pop_back();
	}
}