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

	LinearLayout(BulletWorld * _world, Scene * _scene);

	glm::vec3 getRootPos();
};