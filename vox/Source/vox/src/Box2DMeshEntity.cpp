#pragma once

#include "Box2DMeshEntity.h"
#include "Box2DWorld.h"
#include <MeshInterface.h>

Box2DMeshEntity::Box2DMeshEntity(Box2DWorld * _world, MeshInterface * _mesh, b2BodyType _bodyType, bool _defaultFixture, Shader * _shader, Transform * _transform):
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeBox2DBody(_world, _bodyType, _defaultFixture, _transform),
	MeshEntity(_mesh, transform, _shader)
{
}

Box2DMeshEntity::~Box2DMeshEntity(){
}

void Box2DMeshEntity::update(Step* _step){
	NodeBox2DBody::update(_step);
	MeshEntity::update(_step);
}

b2Fixture * Box2DMeshEntity::createFixture(bool _circle){
	vox::Box bb = calcOverallBoundingBox();
	
	float scaleX = transform->getScaleVector().x;
	float scaleY = transform->getScaleVector().y;
	
	b2FixtureDef d;
	d.density = 1;
	if(!_circle){
		b2Vec2 verts[4];
		verts[0] = b2Vec2(bb.x * scaleX, bb.y * scaleY);
		verts[1] = b2Vec2((bb.x + bb.width) * scaleX, bb.y * scaleY);
		verts[2] = b2Vec2((bb.x + bb.width) * scaleX, (bb.y + bb.height) * scaleY);
		verts[3] = b2Vec2(bb.x * scaleX, (bb.y + bb.height) * scaleY);
		
		b2PolygonShape t;
		t.Set(verts, 4);

		d.shape = &t;
		return body->CreateFixture(&d);
	}else{
		b2CircleShape t;
		t.m_p.x = (bb.x + bb.width*0.5f) * scaleX;
		t.m_p.y = (bb.y + bb.height*0.5f) * scaleY;
		t.m_radius = std::max(bb.width, bb.height) * 0.5f * scaleY;
		
		d.shape = &t;
		return body->CreateFixture(&d);
	}
}