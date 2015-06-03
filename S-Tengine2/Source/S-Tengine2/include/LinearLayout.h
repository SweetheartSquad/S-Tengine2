#pragma once

#include "NodeUI.h"

class NodeUI;

enum Orientation {
	HORIZONTAL,
	VERTICAL
};

class LinearLayout : public NodeUI {
public:
	LinearLayout(Orientation _orientation, BulletWorld * _world, Scene * _scene);
	~LinearLayout();

	float getMeasuredWidth() override;
	float getMeasuredHeight() override;

	void update(Step * _step) override;

	Transform * addChild(NodeUI * _uiElement);
	void setOrientation(Orientation orientation);

private:
	Orientation orientation;
};