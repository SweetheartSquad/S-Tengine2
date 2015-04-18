#pragma once

#include "Box2DMeshEntity.h"
#include "Box2DWorld.h"
#include <MeshInterface.h>

Box2DMeshEntity::Box2DMeshEntity(Box2DWorld * _world, MeshInterface * _mesh, b2BodyType _bodyType, bool _defaultFixture, Shader * _shader, Transform * _transform):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeBox2DBody(_world, _bodyType, _defaultFixture, _transform),
	MeshEntity(_mesh, transform, getShader())
{
}

Box2DMeshEntity::~Box2DMeshEntity(){
}

void Box2DMeshEntity::update(Step* _step){
	NodeBox2DBody::update(_step);
	MeshEntity::update(_step);
}

b2Fixture * Box2DMeshEntity::createFixture(){
	glm::vec3 maxV(-99999, -99999, -99999);
	glm::vec3 minV(99999, 99999, 99999);

	for(auto i : mesh->vertices){
		maxV.x = std::max(i.x, maxV.x);
		maxV.y = std::max(i.y, maxV.y);
		maxV.z = std::max(i.z, maxV.z);

		minV.x = std::min(i.x, minV.x);
		minV.y = std::min(i.y, minV.y);
		minV.z = std::min(i.z, minV.z);
	}

	b2PolygonShape t;
	t.SetAsBox((maxV.x - minV.x) * transform->getScaleVector().x * 0.5f, (maxV.y - minV.y) * transform->getScaleVector().y * 0.5f);
	b2FixtureDef d;
	d.density = 1;
	d.shape = &t;
	return body->CreateFixture(&d);
}