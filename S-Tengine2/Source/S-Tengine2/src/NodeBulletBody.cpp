#pragma once

#include <NodeBulletBody.h>
#include <btBulletDynamicsCommon.h>
#include <Transform.h>
#include <MeshInterface.h>

NodeBulletBody::NodeBulletBody(BulletWorld * _world) :
	world(_world),
	body(nullptr),
	shape(nullptr),
	needsToUpdate(true),
	internalPos(0,0,0)
{
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
			const btTransform & t = body->getWorldTransform();
			const btQuaternion & angle = t.getRotation();
			internalPos = t.getOrigin();
			childTransform->translate(internalPos.x(), internalPos.y(), internalPos.z(), false);

			/*b2Vec2 lv = body->GetLinearVelocity();
			if(maxVelocity.x != -1 && abs(lv.x) > abs(maxVelocity.x)){
				lv.x = maxVelocity.x * (lv.x < 0 ? -1 : 1);
			}
			if(maxVelocity.y != -1 && lv.y > maxVelocity.y){
				lv.y = maxVelocity.y * (lv.y < 0 ? -1 : 1);
			}
			body->SetLinearVelocity(lv);*/

			childTransform->setOrientation(glm::quat(angle.w(), angle.x(), angle.y(), angle.z()));
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

void NodeBulletBody::setColliderAsCapsule(float _radius, float _height){
	assert(shape == nullptr);
	shape = new btCapsuleShape(_radius, _height);
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

void NodeBulletBody::setTranslationPhysical(glm::vec3 _position, bool _relative){
	setTranslationPhysical(_position.x, _position.y, _position.z, _relative);
}
void NodeBulletBody::setTranslationPhysical(float _x, float _y, float _z, bool _relative){
	btVector3 & t = body->getWorldTransform().getOrigin();
	if(_relative){
		t += btVector3(_x, _y, _z);
	}else{
		t = btVector3(_x, _y, _z);
	}
	childTransform->translate(t.x(), t.y(), t.z(), false);
}

void NodeBulletBody::createRigidBody(float _mass, unsigned short int _collisionGroup, unsigned short int _collisionMask){
	assert(shape != nullptr && body == nullptr);
	btTransform t(btQuaternion(0, 0, 0), internalPos);
	btMotionState * motion = new btDefaultMotionState(t);
	btVector3 inertia(0, 0, 0);
	if(_mass != 0 && !shape->isConcave()){
		shape->calculateLocalInertia(_mass, inertia);
	}
	btRigidBody::btRigidBodyConstructionInfo info(_mass, motion, shape, inertia);
	body = new btRigidBody(info);
	world->world->addRigidBody(body, _collisionGroup, _collisionMask);
	body->setUserPointer(this);
}