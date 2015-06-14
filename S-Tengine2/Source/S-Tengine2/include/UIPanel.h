#pragma once

#include <NodeUI.h>

class UIPanel : public NodeUI{
public:
	float width;
	float height;

	UIPanel(float _width, float _height, BulletWorld * _world, Scene * _scene);
	
	float getMeasuredWidth() override;
	float getMeasuredHeight() override;
};