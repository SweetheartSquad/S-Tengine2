#pragma once

#include <BulletWorld.h>
#include <Step.h>

BulletWorld::BulletWorld() :
	collisionConfig(new btDefaultCollisionConfiguration()),
	dispatcher(new btCollisionDispatcher(collisionConfig)),
	broadphase(new btDbvtBroadphase()),
	solver(new btSequentialImpulseConstraintSolver()),
	world(new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig))
{
	world->setGravity(btVector3(0, -20, 0));
}

BulletWorld::~BulletWorld(){
	delete world;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfig;
}

void BulletWorld::update(Step * _step){
	world->stepSimulation(_step->deltaTime);
}