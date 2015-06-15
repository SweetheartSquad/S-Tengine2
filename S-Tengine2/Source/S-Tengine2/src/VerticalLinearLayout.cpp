#pragma once

#include<VerticalLinearLayout.h>

VerticalLinearLayout::VerticalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld)
{
}

void VerticalLinearLayout::update(Step* _step){
	float x = 0;
	float y = 0;
	for(unsigned long int i = 0; i < contents->children.size(); ++i){
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
		x = ui->getMarginLeft();
		y += ui->getMarginBottom();
		trans->translate(x, y, 0.f, false);
		y += ui->getHeight(true, false);
		y += ui->getMarginTop();
	}
	LinearLayout::update(_step);
}

void VerticalLinearLayout::autoResizeHeight(){
	float h = 0.0f;
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				h += node->getHeight(true, true);
			}
		}
	}
	setHeight(h);
}