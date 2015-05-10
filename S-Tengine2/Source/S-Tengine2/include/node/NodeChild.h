#pragma once

#include <node\Node.h>
#include <glm\glm.hpp>
#include <vector>

class Transform;

class NodeChild abstract : public virtual Node{
public:
	/** Reference to this node's parents */
	std::vector<Transform *> parents;

	explicit NodeChild();
	virtual void makeCumulativeModelMatrixDirty();
	bool cumulativeModelMatrixDirty;

	// Returns whether or not _parent is an ancestor of this node (i.e. is its parent, is its parent's parent, etc.)
	// If _parent = nullptr, returns false
	virtual bool hasAncestor(Transform * _parent);
	
	virtual void addParent(Transform * _parent);
	// Loops through the vector of parents and removes the first instance of _parent
	virtual void removeParent(Transform * _parent);

	
	glm::vec3 worldPos;
	// finds the first non-zero ancestor translation vector in the _parent hierarchy and applies all of the matrices of its ancestors to produce world position
	// If there is no transformation data (i.e. parent == nullptr), a zero vector is returned
	virtual glm::vec3 getWorldPos(unsigned long int _parent = 0);
	// Sets the translation vector of the node
	//void setPos(glm::vec3 _pos, bool _convertToRelative = true);

	// Traverses the hierarchy in reverse and returns the inverse model matrix (this * vec4(absolute vector, 1) = relative vector)
	//glm::mat4 getInverseModelMatrixHierarchical();


	// prints the hierarchy to the console in ASCII
	virtual void printHierarchy(unsigned long int _startDepth = 0);

	// Returns the number of Nodes between this node and the top of its hierarchy, following the path through _parent
	virtual unsigned long int calculateDepth(unsigned long int _parent = 0);
};