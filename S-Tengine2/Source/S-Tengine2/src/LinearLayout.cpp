#pragma once

#include <LinearLayout.h>
#include <NodeUI.h>
#include <MeshFactory.h>

LinearLayout::LinearLayout(BulletWorld* _world, Scene* _scene):
	Layout(_world, _scene),
	horizontalAlignment(kLEFT),
	verticalAlignment(kBOTTOM)
{
	setAutoresizeWidth();
	setAutoresizeHeight();
	setBackgroundColour(0.f, 0.f, 0.f, 0.f);
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

void LinearLayout::invalidateLayout(){
	layoutDirty = true;
}

void LinearLayout::update(Step * _step){
	Layout::update(_step);
	if(layoutDirty){
		layoutChildren();
	}
}