#include <Slider.h>

Slider::Slider(BulletWorld* _world, Scene* _scene, float _width, float _height, float _size):
	HorizontalLinearLayout(_world, _scene),
	NodeBulletBody(_world),
	fill(new NodeUI(_world, _scene)),
	size(_size),
	value(_size)
{
	// Slider
	setWidth(_width);
	setHeight(_height);

	setBackgroundColour(-1.f, -1.f, -1.f);

	// Fill (defaults empty)
	fill->setRationalHeight(1.f, this);
	fill->setBackgroundColour(0.f, 0.f, 0.f);

	addChild(fill);
}

void Slider::setValue(float _value){

	value = _value;
	if(value > size){
		value = size;
	}else if(value < 0){
		value = 0;
	}

	updateValue();
}

void Slider::updateValue(){
	if(size > 0){
		float rW = value / size;
		fill->setRationalWidth(value / size);
	}
}

Slider::~Slider(void)
{
}
