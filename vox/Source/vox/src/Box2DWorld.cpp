#pragma once

#include "Box2DWorld.h"
#include "node/NodeBox2DBody.h"
#include "Step.h"

Box2DWorld::Box2DWorld(b2Vec2 _gravityVector):
	NodeUpdatable(),
	b2world(new b2World(_gravityVector)),
	velocityIterations(6),
	positionIterations(2)
{
}

Box2DWorld::~Box2DWorld(){
	delete b2world;
}

void Box2DWorld::update(Step* _step){
	b2world->Step(_step->getDeltaTime(), velocityIterations, positionIterations);
}

void Box2DWorld::addToWorld(NodeBox2DBody * _nodeBox2D){
	if(_nodeBox2D->defaultFixture){
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f * _nodeBox2D->transform->scaleVector.x, 1.0f * _nodeBox2D->transform->scaleVector.y);	
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		if(_nodeBox2D->bodyDef.type != b2_staticBody){
			  fixtureDef.density = 1.0f;
			  fixtureDef.friction = 0.8f;
		}
		_nodeBox2D->body->CreateFixture(&fixtureDef);
	}
}

