#include "Camera.h"
#include <glm/gtc/matrix_transform.inl>

Camera::Camera()
{
	position = new glm::vec3(0.f, 0.f, 5);
	direction = new glm::vec3(0.f, 0.f, 0.f);
	upVector = new glm::vec3(0.f, 0.f, 0.f);
	
	fieldOfView = 65.0f;
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	speed = 0.1f;
	mouseSpeed = 0.005f;

	int *screenHeight = new int();
	int *screenWidth = new int();

	glfwGetWindowSize(glfwGetCurrentContext(), screenWidth, screenHeight);
	
	lasMouseY = *screenHeight/2;
	lastMouseX = *screenWidth/2;

	mouse = &Mouse::getInstance();
	keyboard = &Keyboard::getInstance();
}

Camera::~Camera()
{
}

void Camera::update()
{
	float xOffset = 0.0f;
	float yOffset = 0.0f;

	if(abs(lastMouseX - mouse->mouseX()) > 0.01)
	{
		xOffset = float(320 - mouse->mouseX());	
	}

	if(abs(lasMouseY - mouse->mouseY()) > 0.01)
	{
		yOffset = float(240 - mouse->mouseY());	
	}


	horizontalAngle += mouseSpeed * xOffset;
	verticalAngle += mouseSpeed * yOffset;

	lastMouseX = mouse->mouseX();
	lasMouseY = mouse->mouseY();

	direction = new glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

		// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	// Up vector : perpendicular to both direction and right
	*upVector = glm::cross(right, *direction );

	if (keyboard->keyDown(GLFW_KEY_UP)){
		*position += *direction * speed;
	}
	// Move backward
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		*position -= *direction * speed;
	}
	// Strafe right
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		*position += right * speed;
	}
	// Strafe left
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		*position -= right * speed;
	}

	int *screenHeight = new int();
	int *screenWidth = new int();
	
	glfwGetWindowSize(glfwGetCurrentContext(), screenWidth, screenHeight);
	glfwSetCursorPos(glfwGetCurrentContext(), *screenWidth/2, *screenHeight/2);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(
		*position,           // Camera is here
		*position + *direction, // and looks here : at the same position, plus "direction"
		*upVector            // Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 Camera::getProjectionMatrix()
{
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	return glm::perspective(fieldOfView, 4.0f / 3.0f, 0.1f, 100.0f);
}