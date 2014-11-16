#pragma once

#include "Command.h"
#include <vector>
#include "Tween.h"
#include "Animation.h"

class CMD_AddKeyframe : public Command
{
public:
	CMD_AddKeyframe(Animation * _animation, float _time, float _value, Easing::Type _interpolation);
	~CMD_AddKeyframe();

	void execute();
	void unexecute();

private:

	Animation copyAnimation;

	Tween tween;
	std::vector<Tween> * tweens;

	/*float deltaTime;
	float deltaValue;
	Easing::Type interpolation;*/

	int nextTweenIdx; // -1 if END
	float nextTween_oldDeltaTime;
	float nextTween_oldDeltaValue;

	float nextTween_newDeltaTime;
	float nextTween_newDeltaValue;
	
	int getNextTween(float _time);
	float getTweenEndTime(int _idx);
	float getTweenEndValue(int _idx, float _startValue);

};