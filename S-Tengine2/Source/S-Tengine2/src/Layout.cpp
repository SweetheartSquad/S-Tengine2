#pragma once

#include <Layout.h>

Layout::Layout(BulletWorld* _world) :
	NodeUI(_world)
{}

void Layout::invalidateLayout(){
	layoutDirty = true;
}

void Layout::update(Step * _step){
	if(isLayoutDirty()){
		layoutChildren();
	}
	NodeUI::update(_step);
}