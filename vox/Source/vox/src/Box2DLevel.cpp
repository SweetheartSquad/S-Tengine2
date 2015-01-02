#pragma once

#include "Box2DLevel.h"

Box2DLevel::Box2DLevel():
	NodeUpdatable(),
	world(Box2DWorld(b2Vec2(0, -50)))
{
}

Box2DLevel::~Box2DLevel(){
}

void Box2DLevel::update(Step* _step){
	world.update(_step);
}