#pragma once

#include "node/Node.h"
#include "node/NodeChild.h"
#include "node/NodeTransformable.h"

#include <glm\glm.hpp>

/** Abstract Node with a parent and a transform. Has methods for getting/setting absolute and relative position */
class ShiftKiddie abstract : public virtual NodeChild, public virtual NodeTransformable{
public:

	ShiftKiddie();

	// Returns the translation vector of the node (if _relative is false, applies all of the transformations of the parent nodes before returning the vector)
	glm::vec3 getPos(bool _relative = true);
	// Sets the translation vector of the node
	void setPos(glm::vec3 _pos, bool _convertToRelative = true);

	// Traverses the hierarchy in reverse and returns the inverse model matrix (this * vec4(absolute vector, 1) = relative vector)
	glm::mat4 getInverseModelMatrixHierarchical();
};