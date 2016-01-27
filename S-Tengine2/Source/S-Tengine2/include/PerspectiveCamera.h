#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

/****************************
*
* A perspective camera class, i.e. objects seem smaller the farther they are away from this camera.
*
*****************************/
class PerspectiveCamera : public Camera {
public:
	explicit PerspectiveCamera();
	~PerspectiveCamera();

	// how much the camera slerps from its last orientation to the intended current orientation
	float interpolation;

	/**Tracks the changes in mouse position and uses them to rotate the camera */
	virtual void update(Step * _step) override;
	
	/** The orientation quaternian from the last update loop*/
	glm::quat lastOrientation;

	/**
	* @return The view matrix of the camera
	*/
	virtual glm::mat4 getViewMatrix() const override;

	/**
	* @return The projection matrix of the camera 
	*/
	virtual glm::mat4 getProjectionMatrix() const override;

	// I think this is typically called the center of interest?
	glm::vec3 lookAtSpot;

	glm::vec3 lookAtOffset;
};
