#pragma once

#include "Easing.h"
#include "Tween.h"
#include "NodeUpdatable.h"
#include <vector>

class Step;

class Animation : public NodeUpdatable
{
public:
	explicit Animation(float * _prop);
	~Animation();

	float * prop;

	float startValue;

	float currentTime;
	float referenceValue;
	unsigned long int currentTween;

	std::vector<Tween> tweens;

	void update(Step * _step) override;

	enum LoopType{
		LOOP,
		LOOP_WITH_OFFSET
		//CONSTANT
	} loopType;

private:
	
};