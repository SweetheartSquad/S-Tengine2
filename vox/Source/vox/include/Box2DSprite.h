#pragma once

#include "Sprite.h"
#include "node/NodeBox2D.h"

#include <Box2D/Box2D.h>

#define NO_VELOCITY_LIMIT -1

class Box2DSprite : public Sprite, public NodeBox2D{
public:
	explicit Box2DSprite(b2BodyType _bodyType = b2_dynamicBody, bool _defaultFixture = true, Shader * _shader = nullptr, Transform * _transform = new Transform());
	~Box2DSprite();	

	void update(Step* _step) override;
};