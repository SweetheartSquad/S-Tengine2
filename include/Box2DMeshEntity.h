#pragma once

#include "MeshEntity.h"
#include "node/NodeBox2DBody.h"

class Box2DMeshEntity : public MeshEntity, public NodeBox2DBody{
public:
	explicit Box2DMeshEntity(Box2DWorld * _world, MeshInterface * _mesh = nullptr, b2BodyType _bodyType = b2_dynamicBody, bool _defaultFixture = true, Shader * _shader = nullptr, Transform * _transform = new Transform());
	~Box2DMeshEntity();

	void update(Step* _step) override;
};