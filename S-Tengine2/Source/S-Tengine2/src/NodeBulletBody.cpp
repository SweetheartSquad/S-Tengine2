#pragma once

#include <NodeBulletBody.h>
#include <btBulletDynamicsCommon.h>
#include <Transform.h>
#include <MeshInterface.h>

NodeBulletBody::NodeBulletBody(BulletWorld * _world, bool _default) :
	world(_world),
	body(nullptr),
	shape(nullptr),
	needsToUpdate(true)
{
	// right now this just makes a dynamic unit cube with a mass of 1
	// we'll need to add some stuff to make this properly flexible
	if(_default){
		setColliderAsBox();
		createRigidBody(1);
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
		if(body->isActive() || needsToUpdate){
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

			needsToUpdate = false;
		}
	}
}

void NodeBulletBody::setColliderAsStaticPlane(float _normalX, float _normalY, float _normalZ, float _distanceFromOrigin){
	assert(shape == nullptr);
	shape = new btStaticPlaneShape(btVector3(_normalX, _normalY, _normalZ), _distanceFromOrigin);
}

void NodeBulletBody::setColliderAsBox(float _halfX, float _halfY, float _halfZ){
	assert(shape == nullptr);
	shape = new btBoxShape(btVector3(_halfX, _halfY, _halfZ));
}

void NodeBulletBody::setColliderAsSphere(float _radius){	
	assert(shape == nullptr);
	shape = new btSphereShape(_radius);
}

void NodeBulletBody::setColliderAsMesh(TriMesh * _colliderMesh, bool _convex){
	assert(shape == nullptr);
	if(_convex){
        shape = new btConvexHullShape();
		for (unsigned long int i = 0; i < _colliderMesh->vertices.size(); i++){
            btVector3 v = btVector3(_colliderMesh->vertices[i].x, _colliderMesh->vertices[i].y, _colliderMesh->vertices[i].z);
            static_cast<btConvexHullShape *>(shape)->addPoint(v);
        }
	}else{
		btTriangleMesh * mesh = new btTriangleMesh();
		for(unsigned long int i = 0; i < _colliderMesh->indices.size(); i+=3){
		
			unsigned long int index1 = _colliderMesh->indices.at(i);
			unsigned long int index2 = _colliderMesh->indices.at(i+1);
			unsigned long int index3 = _colliderMesh->indices.at(i+2);

			mesh->addTriangle(
				btVector3(_colliderMesh->vertices.at(index1).x, _colliderMesh->vertices.at(index1).y, _colliderMesh->vertices.at(index1).z),
				btVector3(_colliderMesh->vertices.at(index2).x, _colliderMesh->vertices.at(index2).y, _colliderMesh->vertices.at(index2).z),
				btVector3(_colliderMesh->vertices.at(index3).x, _colliderMesh->vertices.at(index3).y, _colliderMesh->vertices.at(index3).z)
			);
		}
		shape = new btBvhTriangleMeshShape(mesh, true);
	}
}

void NodeBulletBody::createRigidBody(float _mass){
	assert(shape != nullptr);
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0,0,0));
	btMotionState * motion = new btDefaultMotionState(t);
	btVector3 inertia(0,0,0);
	if(_mass != 0 && !shape->isConcave()){
		shape->calculateLocalInertia(_mass, inertia);
	}
	btRigidBody::btRigidBodyConstructionInfo info(_mass, motion, shape, inertia);
	body = new btRigidBody(info);
	world->world->addRigidBody(body);
}