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

	// if target is not nullptr, its value will be set to the slider's value when it updates
	float * target;

	NodeUI * fill;

	// the "handle"
	NodeUI * thumb;

	Slider(BulletWorld* _world,  float _defaultValue, float _valueMax, float _valueMin);
	~Slider(void);

	void update(Step * _step) override;
	
	void setValue(float _value);
	float getValue();
	void updateValue();

	void setStepped(float _valueStep);

	virtual void autoResize() override;

private:
	float value;

	bool controlledByMouse;
};