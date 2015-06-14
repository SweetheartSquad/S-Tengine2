#pragma once

#include <LinearLayout.h>

class HorizontalLinearLayout : public LinearLayout{
public:

	HorizontalLinearLayout(BulletWorld * _bulletWorld, Scene * _scene);

	float getMeasuredWidth() override;
	float getMeasuredHeight() override;

	void update(Step * _step) override;
};