#pragma once

#include "Box2DSprite.h"
#include "Box2DWorld.h"

Box2DSprite::Box2DSprite(Box2DWorld * _world, b2BodyType _bodyType, bool _defaultFixture, Shader* _shader, Transform* _transform):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeBox2DBody(_world, _bodyType, _defaultFixture, _transform)
{
	bodyDef.position.Set(_transform->translationVector.x, _transform->translationVector.y);
	bodyDef.type = _bodyType;
}

Box2DSprite::~Box2DSprite(){
	if(world != nullptr) {
		world->b2world->DestroyBody(body);
	}
}

void Box2DSprite::update(Step * _step){
	NodeBox2DBody::update(_step);
	Sprite::update(_step);
}