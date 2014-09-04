#include "Camera.h"

Camera::Camera(){
	
	this->transform = new Transform();

	transform->translateX(-5);
	
	fieldOfView = 45.0f;
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	speed = 0.1f;
	mouseSpeed = 0.05f;

	Dimension screenDimensions = System::getScreenDimensions();
	
	lastMouseY	= screenDimensions.height/2;
	lastMouseX	= screenDimensions.width/2;

	mouse = &Mouse::getInstance();
}

Camera::~Camera(){
}

void Camera::update(){
	Dimension screenDimensions = System::getScreenDimensions();

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	if(abs(screenDimensions.width*0.5 - mouse->mouseX()) < 0.01){
		offsetX = 0;
	}else{
		offsetX = screenDimensions.width*0.5 - mouse->mouseX();
	}
	
	if(abs(screenDimensions.height*0.5 - mouse->mouseY()) < 0.01){
		offsetY = 0;
	}else{
		offsetY = screenDimensions.height*0.5 - mouse->mouseY();
	}

	float deltaX = lastMouseX - offsetX;
	float deltaY = lastMouseY - offsetY;

	if(deltaX != 0 || deltaY != 0){
		horizontalAngle	= mouseSpeed * vox::deltaTimeCorrection * offsetX;
		verticalAngle	= mouseSpeed * vox::deltaTimeCorrection * offsetY;
		
		transform->orientation = glm::rotate(transform->orientation, verticalAngle, rightVectorLocal);
		transform->orientation = glm::rotate(transform->orientation, horizontalAngle, upVectorLocal);
		
		forwardVectorRotated	= transform->orientation * forwardVectorLocal;
		upVectorRotated			= transform->orientation * upVectorLocal;
		rightVectorRotated		= transform->orientation * rightVectorLocal;
	}

	glfwSetCursorPos(vox::currentContext, screenDimensions.width/2, screenDimensions.height/2);

	this->lastMouseX = offsetX;
	this->lastMouseY = offsetY;
}

glm::mat4 Camera::getViewMatrix(){
	return glm::lookAt(
		transform->translationVector,	// Camera is here        
		transform->translationVector + forwardVectorRotated, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 Camera::getProjectionMatrix(){
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	return glm::perspective(fieldOfView, 4.0f / 3.0f, 0.1f, 100.0f);
}