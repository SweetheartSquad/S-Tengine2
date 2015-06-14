#pragma once

#include <Layout.h>

enum Orientation {
	HORIZONTAL,
	VERTICAL
};

class LinearLayout : public Layout {
public:
	LinearLayout(BulletWorld * _world, Scene * _scene);
	~LinearLayout();
};