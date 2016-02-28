#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PerspectiveCamera.h"
#include <Rectangle.h>

class Mouse;
class Transform;

struct Target{
	glm::vec3 pos;
	NodeChild * target;
	float weight;
	bool active;
};

/****************************
*
* A perspective camera class that will automatically move to frame a list of targets
*
*****************************/
class FollowCamera : public PerspectiveCamera {
public:
	explicit FollowCamera(float _buffer = 0, glm::vec3 _offset = glm::vec3(0,0,0), float _deadZoneX = 0, float _deadZoneY = 0, float _deadZoneZ = 0);
	~FollowCamera();

	/**Tracks the changes in mouse position and uses them to rotate the camera */
	virtual void update(Step * _step) override;
	
	/** The orientation quaternian from the last update loop*/
	glm::quat lastOrientation;
	
	void addTarget(NodeChild * _target, float _weight = 0.f);
	void removeTarget(NodeChild * _target);
	bool hasTarget(NodeChild * _target);

	// Things to follow
	std::vector<Target> targets;
	
	// How much extra space is given around targets
	float buffer;
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

	float targetZoom;

	bool useBounds;
	sweet::Rectangle minBounds;
};