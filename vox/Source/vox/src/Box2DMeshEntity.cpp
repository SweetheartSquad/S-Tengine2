#pragma once

#include "Box2DMeshEntity.h"
#include "Box2DWorld.h"

Box2DMeshEntity::Box2DMeshEntity(Box2DWorld * _world, MeshInterface * _mesh, b2BodyType _bodyType, bool _defaultFixture, Shader * _shader, Transform * _transform):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeBox2DBody(_world, _bodyType, _defaultFixture, _transform),
	MeshEntity(_mesh, transform, shader)
{
}

Box2DMeshEntity::~Box2DMeshEntity(){
}

void Box2DMeshEntity::update(Step* _step){
	NodeBox2DBody::update(_step);
	MeshEntity::update(_step);
}