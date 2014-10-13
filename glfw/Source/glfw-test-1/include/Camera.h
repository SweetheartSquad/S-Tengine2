#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

#include "Mouse.h"
#include "Keyboard.h"
#include "System.h"

class Camera : public Node{
public:
	Camera();
	~Camera();

	/**tracks the changes in mouse position and uses them to rotate the camera */
	void update();

	/** Reference to the mouse singleton */
	Mouse * mouse;

	/** Reference to the camera's transform node */
	Transform * transform;

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

	/** Movement speed multiplier */
	float speed;
	/** Mouse speed multiplier */
	float mouseSpeed;

	glm::quat lastOrientation;

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

private :
	double lastMouseX;
	double lastMouseY;
};