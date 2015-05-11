#pragma once

#include <node\NodeUpdatable.h>
#include <node\NodeChild.h>
#include <BulletWorld.h>

class NodeBulletBody : public virtual NodeUpdatable, public virtual NodeChild{
public:
	BulletWorld * world;
	btRigidBody * body;
	NodeBulletBody(BulletWorld * _world, bool _default);
	~NodeBulletBody();

	void update(Step * _step) override;
};