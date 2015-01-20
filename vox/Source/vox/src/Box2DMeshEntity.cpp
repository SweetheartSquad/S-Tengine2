#pragma once

#include "Box2DMeshEntity.h"

Box2DMeshEntity::Box2DMeshEntity(MeshInterface * _mesh, b2BodyType _bodyType, bool _defaultFixture, Shader * _shader, Transform * _transform):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeBox2DBody(_bodyType, _defaultFixture, _transform),
	MeshEntity(_mesh, transform, shader)
{
}

Box2DMeshEntity::~Box2DMeshEntity(){
}

void Box2DMeshEntity::update(Step* _step){
	NodeBox2DBody::update(_step);
	MeshEntity::update(_step);
}