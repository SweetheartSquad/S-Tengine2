#pragma once

#include <NodeBulletBody.h>
#include <btBulletDynamicsCommon.h>
#include <BulletHeightField.h>
#include <Transform.h>
#include <MeshInterface.h>
#include <Texture.h>

NodeBulletBody::NodeBulletBody(BulletWorld * _world) :
	world(_world),
	body(nullptr),
	shape(nullptr),
	internalPos(0,0,0),
	maxVelocity(-1,-1,-1)
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
	if(body != nullptr && (body->isActive() || directAdjustment)){
		realign();
	}
	NodePhysicsBody::update(_step);
}

void NodeBulletBody::realign(){
	const btTransform & t = body->getWorldTransform();
	const btQuaternion & angle = t.getRotation();
	internalPos = t.getOrigin();
	childTransform->translate(internalPos.x(), internalPos.y(), internalPos.z(), false);

	btVector3 lv = body->getLinearVelocity();
	if(maxVelocity.x() != -1 && abs(lv.x()) > abs(maxVelocity.x())){
		lv.setX(maxVelocity.x() * (lv.x() < 0 ? -1 : 1));
	}if(maxVelocity.y() != -1 && abs(lv.y()) > maxVelocity.y()){
		lv.setY(maxVelocity.y() * (lv.y() < 0 ? -1 : 1));
	}if(maxVelocity.z() != -1 && abs(lv.z()) > maxVelocity.z()){
		lv.setZ(maxVelocity.z() * (lv.z() < 0 ? -1 : 1));
	}
	body->setLinearVelocity(lv);

	childTransform->setOrientation(glm::quat(angle.w(), angle.x(), angle.y(), angle.z()));
	NodePhysicsBody::realign();
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

void NodeBulletBody::setColliderAsHeightMap(Texture * _heightMap, glm::vec3 _scale, unsigned long int _upAxis){
	assert(shape == nullptr);
	assert(_heightMap->storeData);
	assert(_heightMap->channels == 1);

	shape = new BulletHeightFieldShape(_heightMap, _scale, _upAxis);
}

void NodeBulletBody::translatePhysical(glm::vec3 _translation, bool _relative){
	btVector3 & t = body->getWorldTransform().getOrigin();
	if(_relative){
		t += btVector3(_translation.x, _translation.y, _translation.z);
	}else{
		t = btVector3(_translation.x, _translation.y, _translation.z);
	}
	// getOrigin returned a reference to the translation vector, so we only had to modify it
	NodePhysicsBody::translatePhysical(_translation, _relative);
}

void NodeBulletBody::applyLinearImpulse(glm::vec3 _impulse, glm::vec3 _point){
	if(body != nullptr){
		body->activate();
		body->applyImpulse(btVector3(_impulse.x, _impulse.y, _impulse.z), btVector3(_point.x, _point.y, _point.z));
	}
}

void NodeBulletBody::applyForce(glm::vec3 _force, glm::vec3 _point){
	if(body != nullptr){
		body->activate();
		body->applyForce(btVector3(_force.x, _force.y, _force.z), btVector3(_point.x, _point.y, _point.z));
	}
}

void NodeBulletBody::applyLinearImpulseToCenter(glm::vec3 _impulse){
	if(body != nullptr){
		body->activate();
		body->applyCentralImpulse(btVector3(_impulse.x, _impulse.y, _impulse.z));
	}
}

void NodeBulletBody::applyForceToCenter(glm::vec3 _force){
	if(body != nullptr){
		body->activate();
		body->applyCentralForce(btVector3(_force.x, _force.y, _force.z));
	}
}

glm::vec3 NodeBulletBody::getPhysicsBodyCenter(){
	if(body != nullptr){
		btVector3 v = body->getWorldTransform().getOrigin();
		return glm::vec3(v.x(), v.y(), v.z());
	}
}

void NodeBulletBody::rotatePhysical(float _angle, float _x, float _y, float _z, bool _relative){
	btQuaternion q = body->getWorldTransform().getRotation();
	btVector3 axis(_x, _y, _z);
	btScalar angle(glm::radians(_angle));
	if(_relative){
		btQuaternion q2(axis, angle);
		q = q2 * q;
	}else{
		q.setRotation(axis, angle);
	}
	// getRotation didn't return a reference to the quaternion, so we have to set rotation explicitly after modifying it
	body->getWorldTransform().setRotation(q);
	directAdjustment = true;
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