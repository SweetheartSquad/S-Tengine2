#pragma once
#include "Entity.h"

class NodeUI;

enum Orientation {
	HORIZONTAL,
	VERTICAL
};

class LinearLayout : public Transform {
public:

	LinearLayout(Orientation _orientation);
	~LinearLayout();

	void update(Step * _step) override;

	Transform * addChild(NodeChild * _child, bool _underNewTransform = true) override;
	void setOrientation(Orientation orientation);

private:
	bool updateRequired;
	Orientation orientation;
};