#pragma once

#include <HorizontalLinearLayout.h>

HorizontalLinearLayout::HorizontalLinearLayout(BulletWorld* _bulletWorld) :
	LinearLayout(_bulletWorld)
{
}
HorizontalLinearLayout::~HorizontalLinearLayout(){
}

void HorizontalLinearLayout::layoutChildren(){
	glm::vec3 rootPos = getRootPos();
	float x = rootPos.x;
	float y = rootPos.y;

	switch (horizontalAlignment){
	default:
	case kLEFT:
		break;
	case kCENTER:
		x -= getContentsWidth() * 0.5f;
		break;
	case kRIGHT:
		x -= getContentsWidth();
		break;
	}

	for(auto c : uiElements->children){
		Transform * trans = c->asTransform();
		NodeUI * ui = trans->children.at(0)->asNodeUI();
		switch (verticalAlignment){
		default:
		case kBOTTOM:
			break;
		case kMIDDLE:
			y = rootPos.y - ui->getHeight(true, true)*0.5f;
			break;
		case kTOP:
			y = rootPos.y - ui->getHeight(true, true);
			break;
		}

		trans->translate(x, y, 0.f, false);
		x += ui->getWidth(true, true);
	}
}

float HorizontalLinearLayout::getContentsWidth(){
	float w = 0.0f;
	for(auto c : uiElements->children) {
		Transform * trans = c->asTransform();
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = trans->children.at(0)->asNodeUI();
				w += node->getWidth(true, true);
			}
		}
	}
	return w;
}