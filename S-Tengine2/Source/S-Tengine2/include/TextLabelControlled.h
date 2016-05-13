#pragma once

#include <TextLabel.h>

// A text label which is controlled by an external floating point value
// the value is displayed in the text
// the precision of the value displayed and a prefix/suffix can be specified
class TextLabelControlled : public TextLabel{
public:
	float * target;

	// the prefix and suffix are prepended and appended to the target value, respectively
	std::wstring prefix, suffix;

	// how many numbers past the decimal point in the target value are shown in the text
	unsigned long int decimals;

	TextLabelControlled(float * const _target, float _valueMin, float _valueMax, BulletWorld * _world, Font * _font, Shader * _textShader);

	// if _relative is false, sets value to _v
	// otherwise, sets value to value + _v
	float setValue(float _v, bool _relative = false);

	virtual void update(Step * _step) override;

protected:
	// the minimum value of the target
	float valueMin;
	// the maximum value of the target
	float valueMax;
	// the value of the target
	float value;
	// the previous value of the target
	float prevValue;
};