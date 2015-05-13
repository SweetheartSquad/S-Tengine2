#pragma once

#include <node\NodeUpdatable.h>
#include <node\NodeChild.h>
#include <BulletWorld.h>

class TriMesh;

class NodeBulletBody : public virtual NodeUpdatable, public virtual NodeChild{
private:
	bool needsToUpdate;
public:
	BulletWorld * world;
	btRigidBody * body;
	btCollisionShape * shape;
	NodeBulletBody(BulletWorld * _world, bool _default);
	~NodeBulletBody();

	void setColliderAsStaticPlane(float _normalX = 0.f, float _normalY = 1.f, float _normalZ = 0.f, float _distanceFromOrigin = 0.f);
	void setColliderAsBox(float _halfX = 0.5f, float _halfY = 0.5f, float _halfZ = 0.5f);
	void setColliderAsSphere(float _radius = 0.5f);
	void setColliderAsMesh(TriMesh * _colliderMesh, bool _convex);
	// a mass of zero makes it static
	void createRigidBody(float _mass);

	void update(Step * _step) override;
};