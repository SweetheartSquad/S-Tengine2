#pragma once

#include<VerticalLinearLayout.h>

VerticalLinearLayout::VerticalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene)
{
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
		Transform * trans = dynamic_cast<Transform *>(uiElements->children.at(i));
		NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
		
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
		Transform * trans = dynamic_cast<Transform *>(uiElements->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				h += node->getHeight(true, true);
			}
		}
	}
	return h;
}