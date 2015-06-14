#pragma once

#include <LinearLayout.h>

class VerticalLinearLayout : public LinearLayout{
public:

	VerticalLinearLayout(BulletWorld * _bulletWorld, Scene * _scene);

	float getMeasuredWidth() override;
	float getMeasuredHeight() override;

	void update(Step * _step) override;
};