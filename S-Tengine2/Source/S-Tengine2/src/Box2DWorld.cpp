#pragma once

#include <Box2DWorld.h>
#include <node/NodeBox2DBody.h>
#include <Step.h>
#include <Box2DDebugDrawer.h>

Box2DWorld::Box2DWorld(b2Vec2 _gravityVector):
	NodeUpdatable(),
	b2world(new b2World(_gravityVector)),
	velocityIterations(6),
	positionIterations(2),
	timeStepAccumulator(0)
{
}

Box2DWorld::~Box2DWorld(){
	delete b2world;
}

void Box2DWorld::update(Step* _step){
	timeStepAccumulator = _step->getDeltaTime();
	while(timeStepAccumulator >= _step->targetFrameDuration*2){
		b2world->Step(_step->targetFrameDuration, velocityIterations, positionIterations);
		timeStepAccumulator -= _step->targetFrameDuration;
	}
	b2world->Step(timeStepAccumulator, velocityIterations, positionIterations);
	timeStepAccumulator = 0;
}

Box2DDebugDrawer* Box2DWorld::createDebugDrawer() {
	Box2DDebugDrawer * box2dDebug = new Box2DDebugDrawer(this);
	box2dDebug->drawing = true;
	b2world->SetDebugDraw(box2dDebug);
	box2dDebug->AppendFlags(b2Draw::e_shapeBit);
	box2dDebug->AppendFlags(b2Draw::e_centerOfMassBit);
	box2dDebug->AppendFlags(b2Draw::e_jointBit);
	return box2dDebug;

}