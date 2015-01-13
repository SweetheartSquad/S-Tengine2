#pragma once

#include "MeshEntity.h"
#include "node/NodeBox2D.h"

class Box2DMeshEntity : public MeshEntity, public NodeBox2D{
public:
	explicit Box2DMeshEntity(MeshInterface * _mesh = nullptr, b2BodyType _bodyType = b2_dynamicBody, bool _defaultFixture = true, Shader * _shader = nullptr, Transform * _transform = new Transform());
	~Box2DMeshEntity();

	void update(Step* _step) override;
};