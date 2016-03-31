#pragma once

#include<VerticalLinearLayout.h>

VerticalLinearLayout::VerticalLinearLayout(BulletWorld* _bulletWorld) :
	LinearLayout(_bulletWorld)
{
}

VerticalLinearLayout::~VerticalLinearLayout(){
}

void VerticalLinearLayout::layoutChildren(){
	glm::vec3 rootPos = getRootPos();
	float x = rootPos.x;
	float y = rootPos.y;

	switch (verticalAlignment){
	default:
	case kBOTTOM:
		break;
	case kMIDDLE:
		y -= getContentsHeight() * 0.5f;
		break;
	case kTOP:
		y -= getContentsHeight();
		break;
	}

	for(signed long int i = uiElements->children.size()-1; i >= 0; --i){
		Transform * trans = uiElements->children.at(i)->asTransform();
		NodeUI * ui = trans->children.at(0)->asNodeUI();

		switch (horizontalAlignment){
		default:
		case kLEFT:
			break;
		case kMIDDLE:
			x = rootPos.x - ui->getWidth(true, true)*0.5f;
			break;
		case kRIGHT:
			x = rootPos.x - ui->getWidth(true, true);
			break;
		}
		trans->translate(x, y, 0.f, false);
		y += ui->getHeight(true, true);
	}
}

float VerticalLinearLayout::getContentsHeight(){
	float h = 0.0f;
	for(unsigned long int i = 0; i < uiElements->children.size(); ++i) {
		Transform * trans = uiElements->children.at(i)->asTransform();
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = trans->children.at(0)->asNodeUI();
				h += node->getHeight(true, true);
			}
		}
	}
	return h;
}