#pragma once

#include <LinearLayout.h>

class HorizontalLinearLayout : public LinearLayout{
public:

	HorizontalLinearLayout(BulletWorld * _bulletWorld);
	virtual ~HorizontalLinearLayout();

	virtual void layoutChildren() override;
	virtual float getContentsWidth() override;
};