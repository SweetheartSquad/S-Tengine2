#pragma once

#include <node\NodeUpdatable.h>
#include <node\NodeChild.h>
#include <BulletWorld.h>

class TriMesh;

class NodeBulletBody : public virtual NodeUpdatable, public virtual NodeChild{
public:
	BulletWorld * world;
	btRigidBody * body;
	btCollisionShape * shape;
	NodeBulletBody(BulletWorld * _world, bool _default);
	~NodeBulletBody();

	void setColliderAsStaticPlane(float _nX = 0.f, float _nY = 1.f, float _nZ = 0.f, float _planeConstant = 0.f);
	void setColliderAsBox(float _halfX = 0.5f, float _halfY = 0.5f, float _halfZ = 0.5f);
	void setColliderAsSphere(float _radius = 0.5f);
	void setColliderAsMesh(TriMesh * _colliderMesh, bool _convex);
	// a mass of zero makes it static
	void createRigidBody(float _mass);

	void update(Step * _step) override;
};