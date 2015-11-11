#pragma once

#include <Layout.h>

enum VerticalAlignment{
	kBOTTOM,
	kMIDDLE,
	kTOP
};
enum HorizontalAlignment{
	kLEFT,
	kCENTER,
	kRIGHT
};
class LinearLayout : public Layout {
public:
	HorizontalAlignment horizontalAlignment;
	VerticalAlignment verticalAlignment;

	LinearLayout(BulletWorld * _world);

	virtual void update(Step * _step) override;

	virtual void layoutChildren() = 0;

	glm::vec3 getRootPos();

	void invalidateLayout();

private:
	bool layoutValid;
};