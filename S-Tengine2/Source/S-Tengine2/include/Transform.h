#pragma once

#include <iostream>
#include <vector>
#include <iostream>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <node/NodeChild.h>
#include <node/NodeLoadable.h>
#include <node/NodeRenderable.h>
#include <node/NodeUpdatable.h>

typedef enum{
	kWORLD,
	kOBJECT
} CoordinateSpace;

/** A basic transform node */
class Transform : public virtual NodeChild, public virtual NodeLoadable, public virtual NodeRenderable, public virtual NodeUpdatable{
private:
	/** Position */
	glm::vec3 translationVector;
	/** Scale */
	glm::vec3 scaleVector;
	/** Orientation */
	glm::quat orientation;
	
	bool tDirty;
	glm::mat4 tMatrix;
	bool sDirty;
	glm::mat4 sMatrix;
	bool oDirty;
	glm::mat4 oMatrix;
	bool mDirty;
	glm::mat4 mMatrix;

	bool isIdentity;

protected:
public:
	void makeDirty() override;
	
	Transform();
	virtual ~Transform();

	/** 
	* Multiplies the x, y, and z component of the scale vector by _scaleX, _scaleY, and _scaleZ, respectively 
	  If relative = false, sets the scale to the provided values
	*/
	void scale(float _scaleX, float _scaleY, float _scaleZ, bool relative = true);
	
	/** 
	* Multiplies the scale vector by _scale 
	  If relative = false, sets the scale to the provided values
	*/
	void scale(glm::vec3 _scale, bool relative = true);

	/**
	* Adds _translateX, _translateY, and _translateZ to the x, y, and z component of the translation vector, respectively 
	  If relative = false, sets the translation to the provided values
	*/
	void translate(float _translateX, float _translateY, float _translateZ, bool relative = true);
	
	/** 
	* Adds _translate to the translation vector 
	  If relative = false, sets the translation to the provided values
	*/
	void translate(glm::vec3 _translate, bool relative = true);

	/** 
	OBJECT:	Rotates the orientation quaternion by _rotation in object-space (i.e. orientation = _rotation * orientation) 
	WORLD:	Rotates the orientation quaternion by _rotation in world-space (i.e. orientation = orientation * _rotation) 
	*/
	void rotate(glm::quat _rotation, CoordinateSpace _space);
	
	/**
	Rotates the orientation quaternion by the quaternion defined by _angle, _x, _y, and _z (i.e. orientation = quat(axisAngle(_angle, vec3(_x, _y, _z))) * orientation)
	Angle is in degrees
	OBJECT:	Rotates the orientation quaternion by _rotation in object-space (i.e. orientation = _rotation * orientation) 
	WORLD:	Rotates the orientation quaternion by _rotation in world-space (i.e. orientation = orientation * _rotation)
	*/
	void rotate(float _angle, float _x, float _y, float _z, CoordinateSpace _space);
	
	void setOrientation(glm::quat _orientation);
	
	/**
	* Resets the translation, orientation, and scale to their defaults
	*/
	void reset();

	/** 
	* Converts the translation vector to a 4x4 matrix and returns the result 
	*/
	glm::mat4 getTranslationMatrix();

	glm::vec3 getTranslationVector();
	glm::vec3 getScaleVector();
	glm::quat getOrientationQuat();
	
	/** 
	* Converts the scale vector to a 4x4 matrix and returns the result 
	*/
	glm::mat4 getScaleMatrix();
	
	/** 
	* Converts the orientation quaternion to a 4x4 matrix and returns the result 
	*/
	glm::mat4 getOrientationMatrix();
	
	/** Calculates a 4x4 model matrix (translation * orientation * scale) and returns the result */
	glm::mat4 getModelMatrix();




	
	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Renders children,
	* Pops model matrix stack
	*/
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	/** Calls update on all children */
	virtual void update(Step * _step) override;

	/** Calls unload on all children */
	virtual void unload() override;
	/** Calls unload on all children */
	virtual void load() override;


	

	// Child stuff //

	// Inserts _child into this node's list of children at _index
	// Note: does not make any checks, so cyclical references and what not are possible
	virtual void addChildAtIndex(NodeChild * _child, int _index);

	// Erases the node at _index from the list of children
	virtual void removeChildAtIndex(int _index);
	
	// Loops through the node's children and removes the first instance of _child and returns the index
	// Does NOT change node->parent
	// Returns (unsigned long int)(-1) if _child is not a child of this node
	virtual unsigned long int removeChild(NodeChild * _child);
	
	// Returns whether _child is a direct child of this node (use hasDescendant to check hierarchy)
	// If _child = nullptr, returns false
	virtual bool hasChild(NodeChild * _child);
	// Returns whether _child is a descendant of this node (i.e. a child, a child of its children, etc)
	// If _child = nullptr, returns false
	virtual bool hasDescendant(NodeChild * _child);

	/** List of references to child entities */
	std::vector<NodeChild *> children;

	void doRecursively(std::function<void(Node *, void * args[])> _toDo, void * _args[]);

	

	// Calls deleteRecursively on all of _node's children, and then deletes _node
	static void deleteRecursively(Transform * _node);

	// Loops through the node's children and removes the first instance of _child
	// Pushes _child onto the node's children stack
	// Sets _child->parent = this node
	// Note: does nothing if this node is a descendant of _child
	// Returns whether or not the child was successfully added
	virtual bool addChild(NodeChild * _child);

	// Returns the number of Nodes between this node and the top of its hierarchy
	virtual unsigned long int calculateDepth();
};