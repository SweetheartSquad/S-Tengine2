#pragma once

#include <Layout.h>

Layout::Layout(BulletWorld* _world) :
	NodeUI(_world)
{
	setBackgroundColour(0.f, 0.f, 0.f, 0.f);
}

Layout::~Layout(){
}

void Layout::update(Step * _step){
	if(isLayoutDirty()){
		layoutChildren();
	}
	NodeUI::update(_step);
}