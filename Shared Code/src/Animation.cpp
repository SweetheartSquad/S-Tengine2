#pragma once

#include "Animation.h"
#include "Step.h"

Animation::Animation(float * _prop) :
	prop(_prop),
	currentTime(0),
	currentTween(0),
	loopType(LoopType::LOOP),
	referenceValue(0),
	startValue(*_prop)
{
}

Animation::Animation(const Animation & _animation) :
	prop(new float(*_animation.prop)),
	currentTime(_animation.currentTime),
	currentTween(_animation.currentTween),
	loopType(_animation.loopType),
	referenceValue(_animation.referenceValue),
	startValue(_animation.startValue),
	tweens(_animation.tweens)
{
}

void Animation::update(Step * _step){
	if(tweens.size() > 0){
		currentTime += _step->getDeltaTime();
		if(_step->getReverse()){
			while(currentTime < 0){
				currentTime += tweens.at(currentTween)->deltaTime;
				referenceValue -= tweens.at(currentTween)->deltaValue;
				
				if(currentTween == 0){
					switch (loopType){
						case Animation::LOOP:
							referenceValue = startValue;
							for(unsigned long int i = 0; i < tweens.size()-1; ++i){
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
			while(currentTime >= tweens.at(currentTween)->deltaTime){
				currentTime -= tweens.at(currentTween)->deltaTime;
				referenceValue += tweens.at(currentTween)->deltaValue;
				++currentTween;

				if(currentTween >= tweens.size()){
					switch (loopType){
						case Animation::LOOP:
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
		
		/*if(_step->getReverse()){
			*prop = Easing::call(tweens.at(currentTween)->interpolation, tweens.at(currentTween)->deltaTime-currentTime, referenceValue, -tweens.at(currentTween)->deltaValue, tweens.at(currentTween)->deltaTime);
		}else{*/
			*prop = Easing::call(tweens.at(currentTween)->interpolation, currentTime, referenceValue, tweens.at(currentTween)->deltaValue, tweens.at(currentTween)->deltaTime);
		//}
	}
}

Animation::~Animation()
{
}