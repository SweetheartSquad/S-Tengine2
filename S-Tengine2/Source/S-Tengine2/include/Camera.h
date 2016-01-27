#pragma once

#include <Entity.h>
#include <glm\glm.hpp>

/****************************
*
* A basic camera class.
*
*****************************/
class Camera abstract : public Entity {
public:

	Camera();
	~Camera();

	/**Tracks the changes in mouse position and uses them to rotate the camera */
	virtual void update(Step* _step);
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

	/** The camera's vertical field of view */
	float fieldOfView;
	/** The camera's pitch */
	float pitch;
	/** The camera's yaw */
	float yaw;

	float nearClip;
	float farClip;

	/**
	* @return The view matrix of the camera. Pure virtual; to be implemented only in derived classes
	*/
	virtual glm::mat4 getViewMatrix() const = 0;

	/**
	* @return The projection matrix of the camera. Pure virtual; to be implemented only in derived classes
	*/
	virtual glm::mat4 getProjectionMatrix() const = 0;

	// projects the world-space _coords into _screen space (x and y are the pixel coordinates, sign of z indicates whether the camera is facing towards or away from _coords)
	virtual glm::vec3 worldToScreen(glm::vec3 _coords, glm::uvec2 _screen) const;

	// returns an orientation quaternion based on pitch, yaw and roll
	glm::quat calcOrientation() const;
};