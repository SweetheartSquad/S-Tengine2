#pragma once

#include <Slider.h>
#include <Mouse.h>

Slider::Slider(BulletWorld * _world, float _defaultValue, float _valueMin, float _valueMax, bool _horizontal, bool _flipped) :
	NodeUI(_world),
	layout(_horizontal ? (LinearLayout *)new HorizontalLinearLayout(_world) : (LinearLayout *)new VerticalLinearLayout(_world)),
	fill(new NodeUI(_world)),
	thumb(new NodeUI(_world)),
	valueMin(_valueMin),
	valueMax(_valueMax),
	value(0),
	prevValue(0),
	horizontal(_horizontal),
	stepped(false),
	valueStep(0),
	flipped(_flipped)
{
	setBackgroundColour(0.f, 0.f, 0.f);

	if(horizontal){
		layout->horizontalAlignment = kLEFT;
		//setRationalWidth(1.f);
		setHeight(10);

		fill->setRationalHeight(1.f, this);
		thumb->setRationalHeight(1.f, this);
		thumb->setWidth(thumb->getHeight(true, false));
		thumb->background->meshTransform->translate(-0.5f, 0, 0);
		layout->addChild(fill);
		layout->addChild(thumb);
	}else{
		layout->verticalAlignment = kBOTTOM;
		//setRationalHeight(1.f);
		setWidth(10);
		fill->setRationalWidth(1.f, this);
		thumb->setRationalWidth(1.f, this);
		thumb->setHeight(thumb->getWidth(true, false));
		thumb->background->meshTransform->translate(0, -0.5f, 0);
		layout->addChild(thumb);
		layout->addChild(fill);
	}

	fill->setBackgroundColour(1.f, 1.f, 1.f);
	thumb->setBackgroundColour(0.5f, 1.f, 1.f, 0.5f);

	layout->setRationalWidth(1.f, this);
	layout->setRationalHeight(1.f, this);
	addChild(layout);

	setMouseEnabled(true);
	setValue(_defaultValue);
}

void Slider::update(Step * _step){
	// update the slider's value based on the mouse's position when it is being pressed
	if(isDown){
		float v;
		if(horizontal){
			v = (mouse->mouseX() - getWorldPos().x)/getWidth(true, false);
		}else{
			v = (mouse->mouseY() - getWorldPos().y)/getHeight(true, false);
		}
		if(flipped){
			v = 1.f - v;
		}
		v *= (valueMax - valueMin) + valueMin + valueStep*0.5f;
		setValue(v);
	}
	NodeUI::update(_step);
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

void Slider::setValueMax(float _valueMax){
	if(abs(valueMax - _valueMax) < FLT_EPSILON){
		return;
	}

	valueMax = _valueMax;
	setValue(getValue());
}

void Slider::setValueMin(float _valueMin){
	if(abs(valueMin - _valueMin) < FLT_EPSILON){
		return;
	}

	valueMin = _valueMin;
	setValue(getValue());
}

float Slider::getValue(){
	return value;
}

void Slider::updateValue(){
	float v = (value - valueMin) / (valueMax - valueMin);

	// if the value hasn't changed, return early
	float delta = v - prevValue;
	if(abs(delta) < FLT_EPSILON){
		return;
	}
	prevValue = v;

	if(flipped){
		v = 1.f - v;
	}
	if(horizontal){
		fill->setRationalWidth(v, this);
		fill->background->mesh->setUV(2, v, 1.0);
		fill->background->mesh->setUV(1, v, 0.0);
	}else{
		fill->setRationalHeight(v, this);
		fill->background->mesh->setUV(0, 0.0, 1-v);
		fill->background->mesh->setUV(1, 1.0, 1-v);
	}
	fill->background->mesh->dirty = true;
	layout->invalidateLayout();
	fill->invalidateLayout();

	sweet::Event * e = new sweet::Event(std::string("change"));
	e->setFloatData("delta", delta);
	eventManager->triggerEvent(e);
}

void Slider::setStepped(float _valueStep){
	stepped = true;
	valueStep = _valueStep;
}

void Slider::autoResize(){
	NodeUI::autoResize();
	if(horizontal){
		thumb->setWidth(thumb->getHeight(true, false));
	}else{
		thumb->setHeight(thumb->getWidth(true, false));
	}
}

Slider::~Slider(void){
}




SliderController::SliderController(BulletWorld * _world, float * _target, float _defaultValue, float _valueMin, float _valueMax, bool _horizontal, bool _flipped) :
	Slider(_world, _defaultValue, _valueMin, _valueMax, _horizontal, _flipped),
	target(_target)
{
}

void SliderController::update(Step * _step){
	Slider::update(_step);
	*target = value;
}








SliderControlled::SliderControlled(BulletWorld * _world, float * _target, float _valueMin, float _valueMax, bool _horizontal, bool _flipped) :
	Slider(_world, *_target, _valueMin, _valueMax, _horizontal, _flipped),
	target(_target)
{
	setMouseEnabled(false);
}

void SliderControlled::update(Step * _step){
	Slider::update(_step);
	setValue(*target);
}