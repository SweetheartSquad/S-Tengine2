#pragma once

#include <HorizontalLinearLayout.h>

HorizontalLinearLayout::HorizontalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld)
{
}

void HorizontalLinearLayout::update(Step* _step){
	float x = 0;
	for(unsigned long int i = 0; i < contents->children.size(); ++i){
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
		trans->translate(x, 0, 0.f, false);
		x += ui->getWidth(true, true);
	}
	LinearLayout::update(_step);
}

void HorizontalLinearLayout::autoResizeWidth(){
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
	width.measuredSize = w;
}