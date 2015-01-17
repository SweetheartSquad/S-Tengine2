#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

class Mouse;

/****************************
*
* A perspective camera class, i.e. objects seem smaller the farther they are away from this camera.
* For now this also has mouse movement thrown in as well
*
*****************************/
class PerspectiveCamera : public Camera {
public:

	PerspectiveCamera();
	PerspectiveCamera(Transform * trans);
	~PerspectiveCamera();

	/**Tracks the changes in mouse position and uses them to rotate the camera */
	void update(Step * _step) override;
	
	/** The orientation quaternian from the last update loop*/
	glm::quat lastOrientation;

	/**
	* @return The view matrix of the camera
	*/
	glm::mat4 getViewMatrix() override;

	/**
	* @return The projection matrix of the camera 
	*/
	glm::mat4 getProjectionMatrix() override;

	Transform * trans;
};