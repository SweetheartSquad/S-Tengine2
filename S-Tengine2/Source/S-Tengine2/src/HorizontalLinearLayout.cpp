#pragma once

#include <HorizontalLinearLayout.h>

HorizontalLinearLayout::HorizontalLinearLayout(BulletWorld* _bulletWorld, Scene* _scene) :
	LinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld)
{
}

void HorizontalLinearLayout::update(Step* _step){
	float x = 0;
	float y = 0;
	for(unsigned long int i = 0; i < contents->children.size(); ++i){
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		NodeUI * ui = dynamic_cast<NodeUI * >(trans->children.at(0));
		y = ui->getMarginBottom();
		x += ui->getMarginLeft();
		trans->translate(x, y, 0.f, false);
		x += ui->getWidth(true, false);
		x += ui->getMarginRight();
	}
	LinearLayout::update(_step);
}