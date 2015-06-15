#pragma once

#include <LinearLayout.h>
#include <NodeUI.h>
#include <MeshFactory.h>


LinearLayout::LinearLayout(BulletWorld* _world, Scene* _scene):
	Layout(_world, _scene),
	Entity(),
	NodeBulletBody(_world)
{
	autoResizingWidth = autoResizingHeight = true;
}

LinearLayout::~LinearLayout(){
}