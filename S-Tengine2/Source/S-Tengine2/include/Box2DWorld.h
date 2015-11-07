#pragma once

#include "node/NodeUpdatable.h"

#include <Box2D/Box2D.h>

class Box2DDebugDrawer;
class NodeBox2DBody;

class Box2DWorld : public virtual NodeUpdatable{
public:

	b2World * b2world;

	int velocityIterations;
	int positionIterations;

	double timeStepAccumulator;

	explicit Box2DWorld(b2Vec2 _gravityVector = b2Vec2(0.f, -9.8f));
	~Box2DWorld();

	void update(Step* _step) override;

	void addToWorld(NodeBox2DBody * _nodeBox2D, int _userDataGroup = 1);
	
	/**
	* @breif Creates a new Box2DDebugDrawer and attaches it to this Box2DWorld
	* @description Does NOT manage memory for the Box2DDebugDrawer created
	*
	* @return A referece to the Box2DDebugDrawer instance 
	*/
	Box2DDebugDrawer * createDebugDrawer();
};
