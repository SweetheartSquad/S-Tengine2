#pragma once

#include <iostream>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "node/NodeHierarchical.h"

typedef enum{
	kWORLD,
	kOBJECT
} CoordinateSpace;

/** A basic transform node */
class Transform{
private:
	/** Position */
	glm::vec3 translationVector;
	/** Scale */
	glm::vec3 scaleVector;
	/** Orientation */
	glm::quat orientation;
	
public:
	
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

};