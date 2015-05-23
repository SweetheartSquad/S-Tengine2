#pragma once

#include <node\NodeUpdatable.h>
#include <btBulletDynamicsCommon.h>
#include <glm\glm.hpp>

class BulletWorld : public NodeUpdatable{
public:
	// where accurate collision detection code runs
	// default: built-in configuration

	btCollisionConfiguration * collisionConfig;
	btDispatcher * dispatcher;
	// how the world loops through the possible collisions
	// default: dynamic AABB tree implementation
	btBroadphaseInterface * broadphase;
	/** From Bullet sample:
	*	This is what causes the objects to interact properly, taking into account gravity,
	*	game logic supplied forces, collisions, and hinge constraints. It does a good job
	*	as long as you don’t push it to extremes, and is one of the bottlenecks in any high
	*	performance simulation. There are parallel versions available for some threading models.
	*/
	btConstraintSolver * solver;
	btDynamicsWorld * world;
	
	BulletWorld(glm::vec3 _gravity = glm::vec3(0, -9.8, 0));
	~BulletWorld();

	void update(Step * _step) override;
};