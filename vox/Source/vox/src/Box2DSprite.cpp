#pragma once

#include "Box2DSprite.h"

Box2DSprite::Box2DSprite(b2BodyType _bodyType, bool _defaultFixture, Shader* _shader, Transform* _transform):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeBox2D(_bodyType, _defaultFixture, _transform)
{
	bodyDef.position.Set(_transform->translationVector.x, _transform->translationVector.y);
	bodyDef.type = _bodyType;
}

Box2DSprite::~Box2DSprite(){
	
}

void Box2DSprite::update(Step * _step){
	NodeBox2D::update(_step);
	Sprite::update(_step);
}

