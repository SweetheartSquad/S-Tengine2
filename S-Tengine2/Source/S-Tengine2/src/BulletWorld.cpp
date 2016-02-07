#pragma once

#include <BulletWorld.h>
#include <Step.h>

BulletWorld::BulletWorld(glm::vec3 _gravity) :
	collisionConfig(new btDefaultCollisionConfiguration()),
	dispatcher(new btCollisionDispatcher(collisionConfig)),
	broadphase(new btDbvtBroadphase()),
	solver(new btSequentialImpulseConstraintSolver()),
	world(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig)),
	maxSubSteps(1),
	fixedTimeStep(1.f/60.f)
{
	world->setGravity(btVector3(_gravity.x, _gravity.y, _gravity.z));
}

BulletWorld::~BulletWorld(){
	// remove and delete constraints
	for (signed long int i = world->getNumConstraints()-1; i >= 0; i--){
		btTypedConstraint * constraint = world->getConstraint(i);
		world->removeConstraint(constraint);
		delete constraint;
	}

	// remove the and delete rigidbodies/motionstates
	for (signed long int i = world->getNumCollisionObjects()-1; i >= 0; i--){
		btCollisionObject * obj = world->getCollisionObjectArray()[i];
		btRigidBody * body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()){
			delete body->getMotionState();
		}
		world->removeCollisionObject(obj);
		delete obj;
	}

	delete world;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfig;
}

void BulletWorld::update(Step * _step){
	world->stepSimulation(_step->deltaTime, maxSubSteps, fixedTimeStep);
}