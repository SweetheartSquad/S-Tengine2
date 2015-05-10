#include "MousePerspectiveCamera.h"
#include "Mouse.h"
#include "Transform.h"
#include "System.h"

MousePerspectiveCamera::MousePerspectiveCamera() :
	mouse(&Mouse::getInstance()),
	speed(0.1f),
	mouseSpeed(0.05f)
{
	lastMouseY = mouse->mouseX();
	lastMouseX = mouse->mouseY();
}

MousePerspectiveCamera::~MousePerspectiveCamera(){
}

void MousePerspectiveCamera::update(Step* _step){
	double deltaX = 0.0;
	double deltaY = 0.0;

	if(abs((mouse->mouseX(false) + lastMouseX)) > 0.01){
		deltaX = -(mouse->mouseX(false) - lastMouseX);
	}
	if(abs((mouse->mouseY(false) + lastMouseY)) > 0.01){
		deltaY = -(mouse->mouseY(false) - lastMouseY);
	}

	if(deltaY != 0){
		pitch += (mouseSpeed * static_cast<float>(deltaY));
	}if(deltaX != 0){
		yaw += (mouseSpeed * static_cast<float>(deltaX));
	}
	
	lastMouseX = mouse->mouseX(false);
	lastMouseY = mouse->mouseY(false);

	PerspectiveCamera::update(_step);
}