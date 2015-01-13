#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

class Mouse;

/****************************
*
* An orthographic camera class. This camera represents 3d objects in a 2d view.
*
*****************************/
class OrthographicCamera : public Camera {
public:

	OrthographicCamera(float _left, float _right, float _bottom, float _top, float _nearClip, float _farClip);
	~OrthographicCamera();

	void update(Step * _step) override {/* empty*/};

	/**
	* @return The view matrix of the camera
	*/
	glm::mat4 getViewMatrix() override;

	/**
	* @return The projection matrix of the camera 
	*/
	glm::mat4 getProjectionMatrix() override;
	
	float left;
	float top;
	float bottom;
	float right;
	float nearClip;
	float farClip;

};