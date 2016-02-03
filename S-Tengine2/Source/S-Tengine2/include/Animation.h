#pragma once

#include "Easing.h"
#include "Tween.h"
#include "node/NodeUpdatable.h"
#include <vector>

#include "Step.h"

#include <glm/gtx/quaternion.hpp>

template <typename T>
class Animation : public NodeUpdatable{
public:
	float getTweenEndTime(unsigned long int _idx);
	inline T getTweenEndValue(unsigned long int _idx);

	Animation(T * const _prop);
	~Animation();

	T * const prop;

	T startValue;
	bool hasStart;

	// Time elapsed since the start of the animation
	float currentAnimationTime;
	// Time elapsed since the start of the current tween
	float currentTweenTime;
	// The start value of the current tween
	T referenceValue;
	unsigned long int currentTween;

	std::vector<Tween<T> *> tweens;

	inline virtual void update(Step * _step) override;
	inline virtual T ease(Easing::Type _interpolation, float _curTweenTime, T _refValue, T _deltaValue, float _deltaTime);

	enum LoopType{
		kLOOP,
		kLOOP_WITH_OFFSET,
		kCONSTANT
	} loopType;

private:
	
};

template <typename T>
float Animation<T>::getTweenEndTime(unsigned long int _idx){
	if(_idx < tweens.size()){
		float tempTime = 0;
		for(unsigned long int i = 0; i <= _idx; ++i){
			tempTime += tweens.at(i)->deltaTime;
		}
		return tempTime;
	}else{
		throw "index outside bounds of tween array";
	}
};

template<typename T> 
T Animation<T>::getTweenEndValue(unsigned long int _idx){
	if(_idx < tweens.size()){
		T value = startValue;
		for(unsigned long int i = 0; i <= _idx; ++i){
			value += tweens.at(i)->deltaValue;
		}
		return value;
	}else{
		throw "index outside bounds of tween array";
	}
};

template<> 
glm::quat Animation<glm::quat>::getTweenEndValue(unsigned long int _idx){
	if(_idx < tweens.size()){
		glm::quat value = startValue;
		for(unsigned long int i = 0; i <= _idx; ++i){
			value = tweens.at(i)->deltaValue * value;
		}
		return value;
	}else{
		throw "index outside bounds of tween array";
	}
};

template <typename T>
Animation<T>::Animation (T * const _prop) :
	prop(_prop),
	currentAnimationTime(0),
	currentTweenTime(0),
	currentTween(0),
	loopType(LoopType::kLOOP),
	referenceValue(*_prop),
	startValue(*_prop),
	hasStart(false)
{
};

template <typename T>
void Animation<T>::update(Step * _step){
	if(hasStart){
		currentAnimationTime += (float)_step->getDeltaTime();
		//ci::app::console() << "Animation.update() time: " << time << std::endl;
	}
	if(tweens.size() > 0 && (float)_step->getDeltaTime() > 0){
		currentTweenTime += (float)_step->getDeltaTime();
		if(_step->getReverse()){
			while(currentTweenTime <= 0){
				if(currentTween > 0){
					currentTweenTime += tweens.at(currentTween-1)->deltaTime;
					referenceValue -= tweens.at(currentTween-1)->deltaValue;
					--currentTween;
				}else{
					currentTweenTime += tweens.at(tweens.size()-1)->deltaTime;
					referenceValue -= tweens.at(tweens.size()-1)->deltaValue;
					switch (loopType){
						case Animation::kLOOP:
						case Animation::kCONSTANT:
							referenceValue = startValue;
							for(unsigned long int i = 0; i+1 < tweens.size(); ++i){
								referenceValue += tweens.at(i)->deltaValue;
							}
						case Animation::kLOOP_WITH_OFFSET:
							currentTween = tweens.size() - 1;
							break;
						default:
							break;
					}
				}
			}
		}else{
			while(currentTweenTime > tweens.at(currentTween)->deltaTime){
				currentTweenTime -= tweens.at(currentTween)->deltaTime;
				referenceValue += tweens.at(currentTween)->deltaValue;
				if(currentTween < tweens.size()-1){
					++currentTween;
				}else{
					switch (loopType){
						case Animation::kLOOP:
						case Animation::kCONSTANT:
							referenceValue = startValue;
						case Animation::kLOOP_WITH_OFFSET:
							currentTween = 0;
							break;
						default:
							break;
					}
				}
			}
		}
		
		*prop = ease(tweens.at(currentTween)->interpolation, currentTweenTime, referenceValue, tweens.at(currentTween)->deltaValue, tweens.at(currentTween)->deltaTime);
		
		// The time > sum-delta-time bit is inefficient since we don't keep a record of the total animation length (which we should), but it works
		if(loopType == kCONSTANT){
			if(currentAnimationTime < 0){
				*prop = startValue;
				//referenceValue = startValue;
			}else if(currentAnimationTime > getTweenEndTime(tweens.size()-1)){
				*prop = getTweenEndValue(tweens.size()-1);
				//referenceValue = getTweenEndValue(tweens.size()-2);
			}
		}

		// Display the start-frame value on the start frame (ordinarily it would show the last frame-value if looping)
		if(currentAnimationTime == 0){
			*prop = startValue;
			referenceValue = startValue;
			currentTween = 0;
			currentTweenTime = 0;
		}
	}else{
		if(hasStart){
			*prop = startValue;
		}
	}
};

template <>
void Animation<glm::quat>::update(Step * _step){
	if(hasStart){
		currentAnimationTime += (float)_step->getDeltaTime();
		//ci::app::console() << "Animation.update() time: " << time << std::endl;
	}
	if(tweens.size() > 0){
		currentTweenTime += (float)_step->getDeltaTime();
		if(_step->getReverse()){
			while(currentTweenTime <= 0){
				if(currentTween > 0){
					currentTweenTime += tweens.at(currentTween-1)->deltaTime;
					referenceValue = glm::inverse(tweens.at(currentTween-1)->deltaValue) * referenceValue;
					--currentTween;
				}else{
					currentTweenTime += tweens.at(tweens.size()-1)->deltaTime;
					referenceValue = glm::inverse(tweens.at(tweens.size()-1)->deltaValue) * referenceValue;
					switch (loopType){
						case Animation::kLOOP:
						case Animation::kCONSTANT:
							referenceValue = startValue;
							for(unsigned long int i = 0; i+1 < tweens.size(); ++i){
								referenceValue = tweens.at(i)->deltaValue * referenceValue;
							}
						case Animation::kLOOP_WITH_OFFSET:
							currentTween = tweens.size() - 1;
							break;
						default:
							break;
					}
				}
			}
		}else{
			while(currentTweenTime > tweens.at(currentTween)->deltaTime){
				currentTweenTime -= tweens.at(currentTween)->deltaTime;
				referenceValue = tweens.at(currentTween)->deltaValue * referenceValue;
				if(currentTween < tweens.size()-1){
					++currentTween;
				}else{
					switch (loopType){
						case Animation::kLOOP:
						case Animation::kCONSTANT:
							referenceValue = startValue;
						case Animation::kLOOP_WITH_OFFSET:
							currentTween = 0;
							break;
						default:
							break;
					}
				}
			}
		}
		
		*prop = ease(tweens.at(currentTween)->interpolation, currentTweenTime, referenceValue,  tweens.at(currentTween)->deltaValue, tweens.at(currentTween)->deltaTime);
		// The time > sum-delta-time bit is inefficient since we don't keep a record of the total animation length (which we should), but it works
		if(loopType == kCONSTANT){
			if(currentAnimationTime < 0){
				*prop = startValue;
				//referenceValue = startValue;
			}else if(currentAnimationTime > getTweenEndTime(tweens.size()-1)){
				*prop = getTweenEndValue(tweens.size()-1);
				//referenceValue = getTweenEndValue(tweens.size()-2);
			}
		}

		// Display the start-frame value on the start frame (ordinarily it would show the last frame-value if looping)
		if(currentAnimationTime == 0){
			*prop = startValue;
			referenceValue = startValue;
			currentTween = 0;
			currentTweenTime = 0;
		}
	}else{
		if(hasStart){
			*prop = startValue;
		}
	}
};

template <>
glm::quat Animation<glm::quat>::ease(Easing::Type _interpolation, float _curTweenTime, glm::quat _refValue, glm::quat _deltaValue, float _deltaTime) {
	float slerpInterpolation = Easing::call(_interpolation, _curTweenTime, 0, 1, _deltaTime);
	return glm::slerp(_refValue, _deltaValue * _refValue, slerpInterpolation);
}

template <>
glm::vec2 Animation<glm::vec2>::ease(Easing::Type _interpolation, float _curTweenTime, glm::vec2 _refValue, glm::vec2 _deltaValue, float _deltaTime) {
	return glm::vec2(Easing::call(_interpolation, _curTweenTime, _refValue.x, _deltaValue.x, _deltaTime),
		Easing::call(_interpolation, _curTweenTime, _refValue.y, _deltaValue.y, _deltaTime));
}

template <>
glm::vec3 Animation<glm::vec3>::ease(Easing::Type _interpolation, float _curTweenTime, glm::vec3 _refValue, glm::vec3 _deltaValue, float _deltaTime) {
	return glm::vec3(Easing::call(_interpolation, _curTweenTime, _refValue.x, _deltaValue.x, _deltaTime),
		Easing::call(_interpolation, _curTweenTime, _refValue.y, _deltaValue.y, _deltaTime),
		Easing::call(_interpolation, _curTweenTime, _refValue.z, _deltaValue.z, _deltaTime));
}

template <>
glm::vec4 Animation<glm::vec4>::ease(Easing::Type _interpolation, float _curTweenTime, glm::vec4 _refValue, glm::vec4 _deltaValue, float _deltaTime) {
	return glm::vec4(Easing::call(_interpolation, _curTweenTime, _refValue.x, _deltaValue.x, _deltaTime),
		Easing::call(_interpolation, _curTweenTime, _refValue.y, _deltaValue.y, _deltaTime),
		Easing::call(_interpolation, _curTweenTime, _refValue.z, _deltaValue.z, _deltaTime),
		Easing::call(_interpolation, _curTweenTime, _refValue.w, _deltaValue.w, _deltaTime));
}

template <typename T>
T Animation<T>::ease(Easing::Type _interpolation, float _curTweenTime, T _refValue, T _deltaValue, float _deltaTime) {
	return static_cast<T>(Easing::call(_interpolation, _curTweenTime, (float)(_refValue), (float)(_deltaValue), _deltaTime));
}

template <typename T>
Animation<T>::~Animation(){
	while(tweens.size() > 0){
		delete tweens.back();
		tweens.pop_back();
	}
};