#pragma once

#include "MeshEntity.h"
#include "node/NodeBox2DBody.h"

class Box2DMeshEntity : public MeshEntity, public NodeBox2DBody{
public:
	explicit Box2DMeshEntity(Box2DWorld * _world, MeshInterface * _mesh = nullptr, b2BodyType _bodyType = b2_dynamicBody, Shader * _shader = nullptr);

	void update(Step* _step) override;
	
	// if _circle = true, creates a bounding circle
	// else, creates a bounding box
	b2Fixture * createFixture(bool _circle = false);
};