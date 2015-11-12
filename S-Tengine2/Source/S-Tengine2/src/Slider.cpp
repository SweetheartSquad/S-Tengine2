#pragma once

#include <Slider.h>
#include <Mouse.h>

Slider::Slider(BulletWorld * _world, float _defaultValue, float _valueMin, float _valueMax) :
	HorizontalLinearLayout(_world),
	fill(new NodeUI(_world)),
	thumb(new NodeUI(_world)),
	valueMin(_valueMin),
	valueMax(_valueMax),
	value(0),
	stepped(false),
	valueStep(0)
{
	setBackgroundColour(0.f, 0.f, 0.f);

	// Fill (defaults empty)
	fill->setRationalHeight(1.f, this);
	fill->setBackgroundColour(1.f, 1.f, 1.f);
	thumb->setRationalHeight(1.f, this);
	thumb->setWidth(thumb->getHeight(true, false));
	thumb->setBackgroundColour(0.5f, 1.f, 1.f, 0.5f);
	thumb->background->meshTransform->translate(-0.5f, 0, 0);

	addChild(fill);
	addChild(thumb);

	setMouseEnabled(true);
	setValue(_defaultValue);
}

void Slider::update(Step * _step){
	// update the slider's value based on the mouse's position when it is being pressed
	if(isDown){
		setValue((mouse->mouseX() - getWorldPos().x)/getWidth(true, false) * (valueMax - valueMin) + valueMin + valueStep*0.5f);
	}
	HorizontalLinearLayout::update(_step);
}


void Slider::setValue(float _value){
	value = _value;
	
	// if the value is stepped, use the next step instead of the actual value
	if(stepped){
		if(value > 0){
			value -= fmod(value, valueStep);
		}else{
			value += -valueStep - fmod(value, -valueStep);
		}
	}
	
	// clamp the value to a min and max
	if(value > valueMax){
		value = valueMax;
	}else if(value < valueMin){
		value = valueMin;
	}

	updateValue();
}

float Slider::getValue(){
	return value;
}

void Slider::updateValue(){
	float rW = (value - valueMin) / (valueMax - valueMin);

	fill->setRationalWidth(rW, this);
	invalidateLayout();
}

void Slider::setStepped(float _valueStep){
	stepped = true;
	valueStep = _valueStep;
}

void Slider::autoResize(){
	HorizontalLinearLayout::autoResize();
	thumb->setWidth(getHeight(true, false));
}

Slider::~Slider(void){
}




SliderController::SliderController(BulletWorld * _world, float * _target, float _defaultValue, float _valueMin, float _valueMax) :
	Slider(_world, _defaultValue, _valueMin, _valueMax),
	target(_target)
{
}

void SliderController::update(Step * _step){
	Slider::update(_step);
	*target = value;
}








SliderControlled::SliderControlled(BulletWorld * _world, float * _target, float _valueMin, float _valueMax) :
	Slider(_world, *_target, _valueMin, _valueMax),
	target(_target)
{
}

void SliderControlled::update(Step * _step){
	Slider::update(_step);
	setValue(*target);
}