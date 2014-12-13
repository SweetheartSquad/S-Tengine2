#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "NodeTransformable.h"

/****************************
*
* A basic camera class. This camera is rotated via the mouse position 
* The camera's rotation is restricted on the x axis so that It can not flip upside down
*
*****************************/
class Camera abstract : public NodeTransformable {
public:

	Camera();
	~Camera();

	/**Tracks the changes in mouse position and uses them to rotate the camera */
	virtual void update() = 0;

	/** Direction the camera's front would be pointing at if it weren't rotated at all */
	glm::vec3 forwardVectorLocal;
	/** Direction the camera's front is pointing at (local * orientation) */
	glm::vec3 forwardVectorRotated;
	/** Direction the camera's top would be pointing at if it weren't rotated at all */
	glm::vec3 upVectorLocal;
	/** Direction the camera's top is pointing at (local * orientation) */
	glm::vec3 upVectorRotated;
	/** Direction the camera's right side would be pointing at if it weren't rotated at all */
	glm::vec3 rightVectorLocal;
	/** Direction the camera's right side is pointing at (local * orientation) */
	glm::vec3 rightVectorRotated;

	/** The camera's field of view */
	float fieldOfView;
	/** The camera's pitch */
	float pitch;
	/** The camera's yaw */
	float yaw;

	/**
	* @return The view matrix of the camera. Pure virtual; to be implemented only in derived classes
	*/
	virtual glm::mat4 getViewMatrix() = 0;

	/**
	* @return The projection matrix of the camera. Pure virtual; to be implemented only in derived classes
	*/
	virtual glm::mat4 getProjectionMatrix() = 0;
};