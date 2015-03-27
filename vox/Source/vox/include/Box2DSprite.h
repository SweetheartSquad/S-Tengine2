#pragma once

#include "Sprite.h"
#include "node/NodeBox2DBody.h"

#include <Box2D/Box2D.h>

#define NO_VELOCITY_LIMIT -1

class Box2DWorld;
class Texture;

class Box2DSprite : public Sprite, public NodeBox2DBody{
public:
	float width;
	float height;
	float u;
	float v;

	float scale;

	explicit Box2DSprite(Box2DWorld * _world, TextureSampler * _textureSampler, b2BodyType _bodyType = b2_dynamicBody, bool _defaultFixture = true, Shader* _shader = nullptr, Transform* _transform = new Transform(), float _componentScale = 0.005f);
	explicit Box2DSprite(Box2DWorld * _world, b2BodyType _bodyType = b2_dynamicBody, bool _defaultFixture = true, Shader * _shader = nullptr, Transform * _transform = new Transform(), Texture * _texture = nullptr, float _width = 1.f, float _height = 1.f, float _u = 0.f, float _v = 0.f, float _componentScale = 0.005f);
	~Box2DSprite();	

	virtual void update(Step * _step) override;

	virtual float getCorrectedHeight();
	virtual float getCorrectedWidth();

	virtual void createFixture(b2Filter _filter, b2Vec2 _offset = b2Vec2(0.0f, 0.0f), void * _userData = nullptr);

	//Creates a rectangular shape that fits the sprite
	b2PolygonShape getFixtureShape();

	// We seem to need this fairly often
	void setGroupIndex(int16 _groupIndex);

	void setUserData(void * _data);
};