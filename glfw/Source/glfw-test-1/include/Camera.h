#pragma once 

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mouse.h"
#include "Keyboard.h"
#include "System.h"

class Camera{

public:
	Camera();
	~Camera();

	void update();
	
	Keyboard *keyboard;
	Mouse *mouse;

	glm::vec3 *position;
	glm::vec3 *direction;
	glm::vec3 *upVector;
	
	float fieldOfView;
	float verticalAngle;
	float horizontalAngle;

	float speed;
	float mouseSpeed;

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

private :
	double lastMouseX;
	double lasMouseY;
};