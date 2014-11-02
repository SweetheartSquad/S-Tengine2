#pragma once

#include "Node.h"
#include "NodeAnimatable.h"

class Transform;

/** Abstract node with a transform */
class NodeTransformable : public virtual Node{
public:
	explicit NodeTransformable(Transform * _transform) : transform(_transform){};

	/** Reference to this node's transform */
	Transform * transform;
};