#pragma once

#include "Box2DWorld.h"
#include "Box2DSprite.h"

Box2DWorld::Box2DWorld(b2Vec2 _gravityVector):
	NodeUpdatable(),
	world(new b2World(_gravityVector)),
	velocityIterations(6),
	positionIterations(2)
{
}

Box2DWorld::~Box2DWorld(){
	delete world;
}

void Box2DWorld::update(Step* _step){
	world->Step(_step->getDeltaTime(), velocityIterations, positionIterations);
}

void Box2DWorld::addToWorld(Box2DSprite * _sprite){
	_sprite->body = world->CreateBody(&_sprite->bodyDef);
	if(_sprite->defaultFixture){
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f * _sprite->transform->scaleVector.x, 1.0f * _sprite->transform->scaleVector.y);	
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		if(_sprite->bodyDef.type != b2_staticBody){
			  fixtureDef.density = 1.0f;
		}
		_sprite->body->CreateFixture(&fixtureDef);
	}
}

