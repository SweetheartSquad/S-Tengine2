#pragma once

#include <MouseCameraController.h>
#include <Keyboard.h>
#include <MousePerspectiveCamera.h>

#include <glfw/glfw3.h>

MouseCameraController::MouseCameraController(MousePerspectiveCamera * _target) :
	NodeUpdatable(),
	forwardKey(GLFW_KEY_UP),
	backwardsKey(GLFW_KEY_DOWN),
	leftKey(GLFW_KEY_LEFT),
	rightKey(GLFW_KEY_RIGHT),
	target(_target),
	speed(1.0f),
	keyboard(&Keyboard::getInstance())
{
}

void MouseCameraController::update(Step* _step) {
	if(target != nullptr) {
		if(target->firstParent() != nullptr){
			if (keyboard->keyDown(forwardKey)){
				target->parents.at(0)->translate((target->forwardVectorRotated) * speed);
			}
			if (keyboard->keyDown(backwardsKey)){
				target->parents.at(0)->translate((target->forwardVectorRotated) * -speed);
			}
			if (keyboard->keyDown(leftKey)){
				target->parents.at(0)->translate((target->rightVectorRotated) * -speed);
			}
			if (keyboard->keyDown(rightKey)){
				target->parents.at(0)->translate((target->rightVectorRotated) * speed);
			}
		}
	}
}

void MouseCameraController::attach(MousePerspectiveCamera* _target) {
	target = _target;
}

void MouseCameraController::setKeys(unsigned _forwardKey, unsigned _backwardsKey, unsigned _leftKey, unsigned _rightKey) {
	forwardKey = _forwardKey;
	backwardsKey = _backwardsKey;
	leftKey = _leftKey;
	rightKey = _rightKey;
}

void MouseCameraController::setSpeed(float _speed) {
	speed = _speed;
}