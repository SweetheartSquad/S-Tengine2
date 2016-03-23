#pragma once

#include <LinearLayout.h>

class VerticalLinearLayout : public LinearLayout{
public:

	VerticalLinearLayout(BulletWorld * _bulletWorld);
	virtual ~VerticalLinearLayout();

	virtual void layoutChildren() override;
	virtual float getContentsHeight() override;
};