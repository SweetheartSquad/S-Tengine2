#pragma once

#include <node\NodeUpdatable.h>
#include <node\NodeChild.h>

class NodeBulletBody : public virtual NodeUpdatable, public virtual NodeChild{
public:
	NodeBulletBody();
	~NodeBulletBody();
};