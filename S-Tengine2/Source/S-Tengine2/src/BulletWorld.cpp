#pragma once

#include <BulletWorld.h>
#include <Step.h>
#include <Camera.h>

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

NodeBulletBody * BulletWorld::raycast(Camera * _camera, float _range, btCollisionWorld::ClosestRayResultCallback * _rayCallback){
	glm::vec3 pos = _camera->childTransform->getWorldPos();
	btVector3 start(pos.x, pos.y, pos.z);
	btVector3 dir(_camera->forwardVectorRotated.x, _camera->forwardVectorRotated.y, _camera->forwardVectorRotated.z);
	btVector3 end = start + dir*_range;
	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
	if(_rayCallback != nullptr){
		rayCallback.m_collisionFilterGroup = _rayCallback->m_collisionFilterGroup;
		rayCallback.m_collisionFilterMask = _rayCallback->m_collisionFilterMask;
	}
	world->rayTest(start, end, rayCallback);
	
	// store the result in the user's ray callback pointer
	if(_rayCallback != nullptr){
		*_rayCallback = rayCallback;
	}

	// if we're not looking at anything, return nullptr
	if(rayCallback.hasHit()){
		return static_cast<NodeBulletBody *>(rayCallback.m_collisionObject->getUserPointer());
	}else{
		return nullptr;
	}
}