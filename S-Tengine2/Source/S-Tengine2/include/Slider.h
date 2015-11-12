#pragma once
#include <HorizontalLinearLayout.h>

class BulletWorld;
class Scene;

class Slider : public HorizontalLinearLayout{
public:
	bool stepped;
	float valueStep;

	// the minimum value of the slider
	float valueMin;
	// the maximum value of the slider
	float valueMax;

	NodeUI * fill;

	// the "handle"
	NodeUI * thumb;

	Slider(BulletWorld* _world, float _defaultValue, float _valueMin, float _valueMax);
	~Slider();

	virtual void update(Step * _step) override;
	
	void setValue(float _value);
	float getValue();
	void updateValue();

	void setStepped(float _valueStep);

	virtual void autoResize() override;

protected:
	float value;
};

// A slider which modifes the value of a target float
class SliderController : public Slider{
public:
	// the target's value will be set to the slider's value when it updates
	float * target;
	virtual void update(Step * _step) override;

	SliderController(BulletWorld* _world, float * _target, float _defaultValue, float _valueMin, float _valueMax);
};

// A slider which is modified by the value of a target float
class SliderControlled : public Slider{
public:
	// the slider's value will be set to the target's value when it updates
	float * target;
	virtual void update(Step * _step) override;

	SliderControlled(BulletWorld* _world, float * _target, float _valueMin, float _valueMax);
};