#pragma once

#include <CameraController.h>
#include <Camera.h>
#include <Keyboard.h>
#include <Mouse.h>

#include <glfw/glfw3.h>

CameraController::CameraController(Camera * _target) :
	NodeUpdatable(),
	forwardKey(GLFW_KEY_UP),
	backwardsKey(GLFW_KEY_DOWN),
	leftKey(GLFW_KEY_LEFT),
	rightKey(GLFW_KEY_RIGHT),
	target(_target),
	movementSpeed(1.0f),
	rotationSpeed(0.05f),
	keyboard(&Keyboard::getInstance()),
	mouse(&Mouse::getInstance()),
	movementEnabled(true),
	rotationEnabled(true),
	lastMouseX(0),
	lastMouseY(0)
{
	alignMouse();
}

void CameraController::update(Step* _step) {
	if(target != nullptr) {
		// movement controls
		if(movementEnabled){
			Transform * p = target->firstParent();
			if(p != nullptr){
				if (keyboard->keyDown(forwardKey)){
					p->translate((target->forwardVectorRotated) * movementSpeed);
				}if (keyboard->keyDown(backwardsKey)){
					p->translate((target->forwardVectorRotated) * -movementSpeed);
				}if (keyboard->keyDown(leftKey)){
					p->translate((target->rightVectorRotated) * -movementSpeed);
				}if (keyboard->keyDown(rightKey)){
					p->translate((target->rightVectorRotated) * movementSpeed);
				}
			}
		}

		// rotation controls
		if(rotationEnabled){
			if(abs((mouse->mouseX(false) + lastMouseX)) > FLT_EPSILON){
				target->yaw += (rotationSpeed * -(mouse->mouseX(false) - lastMouseX));
			}if(abs((mouse->mouseY(false) + lastMouseY)) > FLT_EPSILON){
				target->pitch -= (rotationSpeed * -(mouse->mouseY(false) - lastMouseY));
			}

			alignMouse();
		}
	}
}

void CameraController::attach(Camera* _target) {
	target = _target;
}

void CameraController::setKeys(unsigned _forwardKey, unsigned _backwardsKey, unsigned _leftKey, unsigned _rightKey) {
	forwardKey = _forwardKey;
	backwardsKey = _backwardsKey;
	leftKey = _leftKey;
	rightKey = _rightKey;
}

void CameraController::alignMouse(){	
	lastMouseX = mouse->mouseX(false);
	lastMouseY = mouse->mouseY(false);
}