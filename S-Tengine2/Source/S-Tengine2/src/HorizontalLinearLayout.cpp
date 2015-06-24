#pragma once

#include <HorizontalLinearLayout.h>

HorizontalLinearLayout::HorizontalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld)
{
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

	for(unsigned long int i = 0; i < contents->children.size(); ++i){
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
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
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				w += node->getWidth(true, true);
			}
		}
	}
	return w;
}