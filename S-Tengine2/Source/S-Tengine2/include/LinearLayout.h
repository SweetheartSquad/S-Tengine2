#pragma once

#include <Layout.h>

class LinearLayout : public Layout {
public:
	LinearLayout(BulletWorld * _world, Scene * _scene);
	~LinearLayout();

	Transform * addChild(NodeUI * _uiElement) override;
};