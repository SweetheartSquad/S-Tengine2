#pragma once

#include <Entity.h>
#include <BulletWorld.h>

class TriMesh;

class NodeBulletBody : public virtual Entity{
protected:
	bool needsToUpdate;
	btVector3 internalPos;
public:
	BulletWorld * world;
	btRigidBody * body;
	btCollisionShape * shape;
	NodeBulletBody(BulletWorld * _world);
	~NodeBulletBody();

	void setColliderAsStaticPlane(float _normalX = 0.f, float _normalY = 1.f, float _normalZ = 0.f, float _distanceFromOrigin = 0.f);
	void setColliderAsBox(float _halfX = 0.5f, float _halfY = 0.5f, float _halfZ = 0.5f);
	void setColliderAsSphere(float _radius = 0.5f);
	void setColliderAsCapsule(float _radius = 0.5f, float _height = 0.5f);
	void setColliderAsMesh(TriMesh * _colliderMesh, bool _convex);
	// a mass of zero makes it static
	virtual void createRigidBody(float _mass, unsigned short int _collisionGroup = -1, unsigned short int _collisionMask = -1);
	
	// just calls the version with three floats instead of a vec3
	void setTranslationPhysical(glm::vec3 _position, bool _relative = false);
	virtual void setTranslationPhysical(float _x, float _y, float _z, bool _relative = false);

	virtual void update(Step * _step) override;
};