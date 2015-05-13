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

	void setAsBox(float _halfX = 0.5f, float _halfY = 0.5f, float _halfZ = 0.5f);
	void setAsSphere(float _radius = 0.5f);
	void setAsMesh(TriMesh * _colliderMesh, bool _convex);
	void createRigidBody();

	void update(Step * _step) override;
};