#pragma once

#include <LinearLayout.h>

class VerticalLinearLayout : public LinearLayout{
public:

	VerticalLinearLayout(BulletWorld * _bulletWorld, Scene * _scene);

	virtual void update(Step * _step) override;
	virtual void autoResizeHeight() override;
};