#pragma once

#include "Box2DMeshEntity.h"

class Texture;

class RandomGround : public Box2DMeshEntity{
public:
	explicit RandomGround(Box2DWorld * _world, unsigned long int _numPoints, float _threshold, Texture * _texture = nullptr, float _width = 1.f, float _height = 1.f);
	~RandomGround();
};