#pragma once

#include "node/Node.h"
#include "node/NodeChild.h"
#include "node/NodeTransformable.h"

#include <glm\glm.hpp>

/** Abstract Node with a parent and a transform. Has methods for getting/setting absolute and relative position */
class ShiftKiddie abstract : public virtual NodeChild, public virtual NodeTransformable{
public:

	ShiftKiddie();

};