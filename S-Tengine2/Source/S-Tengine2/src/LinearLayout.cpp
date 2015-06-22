#pragma once

#include <LinearLayout.h>
#include <NodeUI.h>
#include <MeshFactory.h>

LinearLayout::LinearLayout(BulletWorld* _world, Scene* _scene):
	Layout(_world, _scene),
	Entity(),
	NodeBulletBody(_world),
	horizontalAlignment(kLEFT),
	verticalAlignment(kBOTTOM)
{
	setAutoresizeWidth();
	setAutoresizeHeight();
}

glm::vec3 LinearLayout::getRootPos(){
	glm::vec3 res(0);
	
	switch (horizontalAlignment){
		default:
		case kLEFT:
			break;
		case kCENTER:
			res.x = getWidth() * 0.5f;
			break;
		case kRIGHT:
			res.x = getWidth();
			break;
	}
	switch (verticalAlignment){
		default:
		case kBOTTOM:
			break;
		case kMIDDLE:
			res.y = getHeight() * 0.5f;
			break;
		case kTOP:
			res.y = getHeight();
			break;
	}

	return res;
}

void LinearLayout::update(Step * _step){
	Layout::update(_step);
	layoutChildren();
}