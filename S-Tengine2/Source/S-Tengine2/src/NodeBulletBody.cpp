#pragma once

#include <NodeBulletBody.h>
#include <btBulletDynamicsCommon.h>
#include <Transform.h>

NodeBulletBody::NodeBulletBody(BulletWorld * _world, bool _default) :
	world(_world),
	body(nullptr)
{
	// right now this just makes a dynamic unit cube with a mass of 1
	// we'll need to add some stuff to make this properly flexible
	if(_default){
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(0,0,0));
		btBoxShape * shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
		btMotionState * motion = new btDefaultMotionState(t);
		btVector3 inertia(0,0,0);
		float mass = 1;
		if(mass != 0){
			shape->calculateLocalInertia(mass, inertia);
		}
		btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
		body = new btRigidBody(info);
		world->world->addRigidBody(body);
	}
}

NodeBulletBody::~NodeBulletBody(){
	if(world != nullptr && body != nullptr) {
		world->world->removeRigidBody(body);
		body = nullptr;
		world = nullptr;
	}
}

void NodeBulletBody::update(Step * _step){
	if(body != nullptr){
		if(body->isActive()){
			btTransform t = body->getWorldTransform();
			btVector3 pos = t.getOrigin();
			btQuaternion angle = t.getRotation();
			parents.at(0)->translate(pos.x(), pos.y(), pos.z(), false);
			
			/*b2Vec2 lv = body->GetLinearVelocity();
			if(maxVelocity.x != -1 && abs(lv.x) > abs(maxVelocity.x)){
				lv.x = maxVelocity.x * (lv.x < 0 ? -1 : 1);
			}
			if(maxVelocity.y != -1 && lv.y > maxVelocity.y){
				lv.y = maxVelocity.y * (lv.y < 0 ? -1 : 1);
			}
			body->SetLinearVelocity(lv);*/
			
			parents.at(0)->setOrientation(glm::quat(angle.w(), angle.x(), angle.y(), angle.z()));
		}
	}
}