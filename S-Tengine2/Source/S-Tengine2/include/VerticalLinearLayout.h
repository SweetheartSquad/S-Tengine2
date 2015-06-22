#pragma once

#include <LinearLayout.h>

class VerticalLinearLayout : public LinearLayout{
public:

	VerticalLinearLayout(BulletWorld * _bulletWorld, Scene * _scene);

	virtual void layoutChildren() override;
	virtual float getContentsHeight() override;
};