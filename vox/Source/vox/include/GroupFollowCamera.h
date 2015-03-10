#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PerspectiveCamera.h"

class Mouse;
class Sprite;
class Entity;
/****************************
*
* A perspective camera class, i.e. objects seem smaller the farther they are away from this camera.
* For now this also has mouse movement thrown in as well
*
*****************************/
class GroupFollowCamera : public PerspectiveCamera {
public:
	

	/** trans is the target for following with lookAt */
	explicit GroupFollowCamera(glm::vec3 _offset = glm::vec3(0,0,0), float _deadZoneX = 0, float _deadZoneY = 0, float _deadZoneZ = 0);
	~GroupFollowCamera();

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
	

	// Sprite to follow
	Entity * trans;
	// Look at the sprite offset by this
	glm::vec3 offset;
	// Tolerable horizontal difference between the follow target and the current lookAt
	float deadZoneX;
	// Tolerable vertical difference between the follow target and the current lookAt
	float deadZoneY;
	// Tolerable depth difference between the follow target and the current lookAt
	float deadZoneZ;

	glm::vec3 lookAtSpot;
	
};
//  zoom = max(max(w/width, h/height) + 0.1, 10) + zoomModifier;