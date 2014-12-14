#pragma once

#include "Node.h"

class Transform;

/** Abstract node with a transform */
class NodeTransformable abstract : public virtual Node{
public:
	explicit NodeTransformable(Transform * _transform) : transform(_transform){};
	
	/** Reference to this node's transform */
	Transform * transform;
};