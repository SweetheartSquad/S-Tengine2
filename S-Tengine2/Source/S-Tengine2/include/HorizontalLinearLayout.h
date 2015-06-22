#pragma once

#include <LinearLayout.h>

class HorizontalLinearLayout : public LinearLayout{
public:

	HorizontalLinearLayout(BulletWorld * _bulletWorld, Scene * _scene);

	virtual void layoutChildren() override;
	virtual float getContentsWidth() override;
};