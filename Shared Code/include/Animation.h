#pragma once

#include "Easing.h"
#include "Tween.h"
#include "NodeUpdatable.h"
#include <vector>

class Step;

class Animation : public NodeUpdatable
{
public:
	float getTweenEndTime(unsigned long int _idx);
	float getTweenEndValue(unsigned long int _idx);

	explicit Animation(float * const _prop);
	~Animation();

	float * const prop;

	float startValue;
	bool hasStart;

	// Time elapsed since the start of the animation
	float time;
	// Time elapsed since the start of the current tween
	float currentTime;
	// The start value of the current tween
	float referenceValue;
	unsigned long int currentTween;

	std::vector<Tween *> tweens;

	void update(Step * _step) override;

	enum LoopType{
		LOOP,
		LOOP_WITH_OFFSET,
		CONSTANT
	} loopType;

private:
	
};