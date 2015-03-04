#pragma once

#include "Box2DMeshEntity.h"

class RandomGround : public Box2DMeshEntity{
public:
	explicit RandomGround(Box2DWorld * _world, int _numPoints, float _threshold);
	~RandomGround();
};