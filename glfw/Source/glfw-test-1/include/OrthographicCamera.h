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

	OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
	~OrthographicCamera();

	void update() { /* empty*/ };

	/**
	* @return The view matrix of the camera
	*/
	glm::mat4 getViewMatrix();

	/**
	* @return The projection matrix of the camera 
	*/
	glm::mat4 getProjectionMatrix();
	
private:
	
	float left;
	float top;
	float bottom;
	float right;
	float near;
	float far;

};