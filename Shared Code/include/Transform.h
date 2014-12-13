#pragma once

#include <iostream>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "NodeHierarchical.h"

typedef enum{
	kWORLD,
	kOBJECT
} CoordinateSpace;

/** A basic transform node */
class Transform: public NodeHierarchical{
public:
	/** Position */
	glm::vec3 translationVector;
	/** Scale */
	glm::vec3 scaleVector;
	/** Orientation */
	glm::quat orientation;

	Transform();
	virtual ~Transform();

	/** Calls render on children (recursive) (why does a transform node need a render method?) */
	virtual void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix);
	/** Doesn't do anything (should probably call update on children) */
	virtual void update();

	/** Multiplies the x, y, and z component of the scale vector by _scaleX, _scaleY, and _scaleZ, respectively */
	void scale(float _scaleX, float _scaleY, float _scaleZ);
	/** Multiplies the scale vector by _scale */
	void scale(glm::vec3 _scale);
	/** Multiplies the x component of the scale vector by _scaleX */
	void scaleX(float _scaleX);
	/** Multiplies the y component of the scale vector by _scaleY */
	void scaleY(float _scaleY);
	/** Multiplies the z component of the scale vector by _scaleZ */
	void scaleZ(float _scaleZ);

	/** Adds _translateX, _translateY, and _translateZ to the x, y, and z component of the translation vector, respectively */
	void translate(float _translateX, float _translateY, float _translateZ);
	/** Adds _translate to the translation vector */
	void translate(glm::vec3 _translate);

	/** 
	OBJECT:	Rotates the orientation quaternion by _rotation in object-space (i.e. orientation = _rotation * orientation) 
	WORLD:	Rotates the orientation quaternion by _rotation in world-space (i.e. orientation = orientation * _rotation) 
	*/
	void rotate(glm::quat _rotation, CoordinateSpace _space);
	/**
	Rotates the orientation quaternion by the quaternion defined by _angle, _x, _y, and _z (i.e. orientation = quat(axisAngle(_angle, vec3(_x, _y, _z))) * orientation)
	OBJECT:	Rotates the orientation quaternion by _rotation in object-space (i.e. orientation = _rotation * orientation) 
	WORLD:	Rotates the orientation quaternion by _rotation in world-space (i.e. orientation = orientation * _rotation)
	*/
	void rotate(float _angle, float _x, float _y, float _z, CoordinateSpace _space);

	/** Converts the translation vector to a 4x4 matrix and returns the result */
	glm::mat4 getTranslationMatrix();
	/** Converts the scale vector to a 4x4 matrix and returns the result */
	glm::mat4 getScaleMatrix();
	/** Converts the orientation quaternion to a 4x4 matrix and returns the result */
	glm::mat4 getOrientationMatrix();
	/** Calculates a 4x4 model matrix (translation * orientation * scale) and returns the result */
	glm::mat4 getModelMatrix();

	void addChild(Transform * _child);
	virtual void removeChildAtIndex(int _index);
	void setParent(Transform * _parent);
};