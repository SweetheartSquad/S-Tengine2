#pragma once

#include <node\Node.h>
#include <glm\glm.hpp>

class Transform;

class NodeChild abstract : public virtual Node{
protected:
public:
	virtual void makeDirty();
	bool transformDirty;
	glm::vec3 worldPos;


	/** Reference to this node's parent */
	Transform * parent;

	explicit NodeChild(Transform * _parent = nullptr);

	// Returns whether or not _parent is an ancestor of this node (i.e. is its parent, is its parent's parent, etc.)
	// If _parent = nullptr, returns false
	virtual bool hasAncestor(Transform * _parent);


	// Returns the translation vector of the node (if _relative is false, applies all of the transformations of the parent nodes before returning the vector)
	// If there is no transformation data (i.e. parent == nullptr) a zero vector is returned
	glm::vec3 getWorldPos();
	// Sets the translation vector of the node
	void setPos(glm::vec3 _pos, bool _convertToRelative = true);

	// Traverses the hierarchy in reverse and returns the inverse model matrix (this * vec4(absolute vector, 1) = relative vector)
	glm::mat4 getInverseModelMatrixHierarchical();
};