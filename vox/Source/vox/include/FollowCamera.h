#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PerspectiveCamera.h"

class Mouse;
class ShiftKiddie;
/****************************
*
* A perspective camera class that will automatically move to frame a list of targets
*
*****************************/
class FollowCamera : public PerspectiveCamera {
public:
	explicit FollowCamera(glm::vec3 _offset = glm::vec3(0,0,0), float _deadZoneX = 0, float _deadZoneY = 0, float _deadZoneZ = 0);
	~FollowCamera();

	/**Tracks the changes in mouse position and uses them to rotate the camera */
	void update(Step * _step) override;
	
	/** The orientation quaternian from the last update loop*/
	glm::quat lastOrientation;

	/**
	* @return The view matrix of the camera
	*/
	glm::mat4 getViewMatrix() override;
	
	void addTarget(ShiftKiddie * _target);
	void removeTarget(ShiftKiddie * _target);

	// Things to follow
	std::vector<ShiftKiddie *> targets;
	
	// Look at the sprite offset by this
	glm::vec3 offset;
	// Tolerable horizontal difference between the follow targets and the current lookAt
	float deadZoneX;
	// Tolerable vertical difference between the follow targets and the current lookAt
	float deadZoneY;
	// Tolerable depth difference between the follow targets and the current lookAt
	float deadZoneZ;

	// Minimum zoom
	float minimumZoom;
};