#pragma once

#include "PerspectiveCamera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "System.h"
#include "Transform.h"

PerspectiveCamera::PerspectiveCamera():
	Camera(),
	NodeTransformable(new Transform),
	NodeAnimatable(),
	NodeUpdatable(),
	mouse(&Mouse::getInstance()),
	speed(0.1f),
	mouseSpeed(0.05f),
	lastOrientation(1.f, 0.f, 0.f, 0.f)
{
	transform->translate(-5.f, 0.f, 0.f);
	transform->translate(0.f, 3.f, 0.f);

	Dimension screenDimensions = vox::getScreenDimensions();
	lastMouseY   = screenDimensions.height/2;
	lastMouseX	= screenDimensions.width/2;
}

PerspectiveCamera::~PerspectiveCamera(){
}

void PerspectiveCamera::update(Step * _step){
	lastOrientation = transform->orientation;

	Dimension screenDimensions = vox::getScreenDimensions();

	double centerX = static_cast<double>(screenDimensions.width)*0.5;
	double centerY = static_cast<double>(screenDimensions.height)*0.5;

	double offsetX = 0.;
	double offsetY = 0.;

	if(abs(centerX - mouse->mouseX()) > 0.01){
		offsetX = centerX - mouse->mouseX();
	}
	if(abs(centerY - mouse->mouseY()) > 0.01){
		offsetY = centerY - mouse->mouseY();
	}

	double deltaX = lastMouseX - offsetX;
	double deltaY = lastMouseY - offsetY;

	if(deltaX != 0){
		pitch += (mouseSpeed * static_cast<float>(offsetY));
	}if(deltaY != 0){
		yaw += (mouseSpeed * static_cast<float>(offsetX));
	}

	//restriction
	if(pitch < -80.f){
		pitch = -80.f;
	}else if(pitch > 80.f){
		pitch = 80.f;
	}

	transform->orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	transform->orientation = glm::rotate(transform->orientation, yaw, upVectorLocal);
	transform->orientation = glm::rotate(transform->orientation, pitch, rightVectorLocal);

	transform->orientation = glm::slerp(lastOrientation, transform->orientation, 0.15f * static_cast<float>(vox::deltaTimeCorrection));

	forwardVectorRotated   = transform->orientation * forwardVectorLocal;
	rightVectorRotated	   = transform->orientation * rightVectorLocal;
	upVectorRotated		   = transform->orientation * upVectorLocal;

	glfwSetCursorPos(vox::currentContext, centerX, centerY);

	lastMouseX = offsetX;
	lastMouseY = offsetY;
}

glm::mat4 PerspectiveCamera::getViewMatrix(){
	return glm::lookAt(
		transform->translationVector,	// Camera is here
		transform->translationVector + forwardVectorRotated, // and looks here : at the same position, plus "direction"
		upVectorRotated				// Head is up (set to 0,-1,0 to look upside-down)
		);
}

glm::mat4 PerspectiveCamera::getProjectionMatrix(){
	Dimension screenDimensions = vox::getScreenDimensions();
	// Projection matrix : 45° Field of View, ratio, near-far clip : 0.1 unit <-> 100 units
	return glm::perspective(fieldOfView, static_cast<float>(screenDimensions.width)/static_cast<float>(screenDimensions.height), 0.1f, 100.0f);
}