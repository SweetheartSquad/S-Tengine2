#pragma once
#include <HorizontalLinearLayout.h>

class BulletWorld;
class Scene;

class Slider :
	public HorizontalLinearLayout
{
public:
	float size;

	NodeUI * fill;

	Slider(BulletWorld* _world, Scene* _scene, float _width, float _height, float _size);
	~Slider(void);

	void setValue(float _value);
	void updateValue();

private:
	float value;
};

