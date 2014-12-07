#pragma once

#include "Node.h"
#include "NodeChild.h"
#include "NodeTransformable.h"

#include <glm\glm.hpp>

class ShiftKiddie abstract : public virtual NodeChild, public virtual NodeTransformable{
public:

	ShiftKiddie();

	// Returns the translation vector of the node (if _relative is false, applies all of the transformations of the parent nodes before returning the vector)
	glm::vec3 getPos(bool _relative = true);
	// Sets the translation vector of the node
	void setPos(glm::vec3 _pos, bool _convertToRelative = true);
};