#pragma once

#include "node/NodeUpdatable.h"

#include <Box2D/Box2D.h>

class NodeBox2DBody;

class Box2DWorld : public virtual NodeUpdatable{
public:

	b2World * b2world;

	int velocityIterations;
	int positionIterations;

	float timeStepAccumulator;

	explicit Box2DWorld(b2Vec2 _gravityVector);
	~Box2DWorld();

	void update(Step* _step) override;

	void addToWorld(NodeBox2DBody * _nodeBox2D, int _userDataGroup = 1);
};
