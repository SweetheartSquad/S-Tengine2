#pragma once

#include <JoystickVirtual.h>
#include <GLFW\glfw3.h>

JoystickVirtual::JoystickVirtual(int _id) :
	Joystick(_id),
	keyboard(&Keyboard::getInstance())
{
	name = "Virtual Joystick";
	
	axisLeftUp = GLFW_KEY_W;
	axisLeftDown = GLFW_KEY_S;
	axisLeftLeft = GLFW_KEY_A;
	axisLeftRight = GLFW_KEY_D;

	dpadUp = GLFW_KEY_W;
	dpadDown = GLFW_KEY_S;
	dpadLeft = GLFW_KEY_A;
	dpadRight = GLFW_KEY_D;

	axisRightUp = GLFW_KEY_I;
	axisRightDown = GLFW_KEY_K;
	axisRightLeft = GLFW_KEY_J;
	axisRightRight = GLFW_KEY_L;

	bumperLeft = GLFW_KEY_Q;
	bumperRight = GLFW_KEY_E;

	axisTriggerLeft = GLFW_KEY_Z;
	axisTriggerRight = GLFW_KEY_C;

	centerButtonLeft = GLFW_KEY_SPACE;
	centerButtonRight = GLFW_KEY_ESCAPE;

	faceButtonUp = GLFW_KEY_KP_8;
	faceButtonDown = GLFW_KEY_KP_2;
	faceButtonLeft = GLFW_KEY_KP_4;
	faceButtonRight = GLFW_KEY_KP_6;
}

void JoystickVirtual::update(Step * _step){
	// left stick
	axesValues[xbox_axes::kLY] = 0.f;
	if(keyboard->keyDown(axisLeftUp)){
		axesValues[xbox_axes::kLY] -= 1.f;
	}if(keyboard->keyDown(axisLeftDown)){
		axesValues[xbox_axes::kLY] += 1.f;
	}
	
	axesValues[xbox_axes::kLX] = 0.f;
	if(keyboard->keyDown(axisLeftLeft)){
		axesValues[xbox_axes::kLX] -= 1.f;
	}if(keyboard->keyDown(axisLeftRight)){
		axesValues[xbox_axes::kLX] += 1.f;
	}

	// right stick
	axesValues[xbox_axes::kRY] = 0.f;
	if(keyboard->keyDown(axisRightUp)){
		axesValues[xbox_axes::kRY] -= 1.f;
	}if(keyboard->keyDown(axisRightDown)){
		axesValues[xbox_axes::kRY] += 1.f;
	}
	
	axesValues[xbox_axes::kRX] = 0.f;
	if(keyboard->keyDown(axisRightLeft)){
		axesValues[xbox_axes::kRX] -= 1.f;
	}if(keyboard->keyDown(axisRightRight)){
		axesValues[xbox_axes::kRX] += 1.f;
	}
	
	// triggers
	axesValues[xbox_axes::kTRIGGERS] = 0.f;
	if(keyboard->keyDown(axisTriggerLeft)){
		axesValues[xbox_axes::kTRIGGERS] -= 1.f;
	}if(keyboard->keyDown(axisTriggerRight)){
		axesValues[xbox_axes::kTRIGGERS] += 1.f;
	}

	// bumpers
	if(keyboard->keyJustDown(bumperLeft)){
		buttonDownListener(xbox_buttons::kL1);
	}else{
		buttonUpListener(xbox_buttons::kL1);
	}if(keyboard->keyJustDown(bumperRight)){
		buttonDownListener(xbox_buttons::kR1);
	}else{
		buttonUpListener(xbox_buttons::kR1);
	}

	// face buttons
	if(keyboard->keyDown(faceButtonUp)){
		buttonDownListener(xbox_buttons::kY);
	}else{
		buttonUpListener(xbox_buttons::kY);
	}if(keyboard->keyDown(faceButtonDown)){
		buttonDownListener(xbox_buttons::kA);
	}else{
		buttonUpListener(xbox_buttons::kA);
	}if(keyboard->keyDown(faceButtonLeft)){
		buttonDownListener(xbox_buttons::kX);
	}else{
		buttonUpListener(xbox_buttons::kX);
	}if(keyboard->keyDown(faceButtonRight)){
		buttonDownListener(xbox_buttons::kB);
	}else{
		buttonUpListener(xbox_buttons::kB);
	}

	// center buttons
	if(keyboard->keyDown(faceButtonRight)){
		buttonDownListener(xbox_buttons::kB);
	}else{
		buttonUpListener(xbox_buttons::kB);
	}if(keyboard->keyDown(faceButtonRight)){
		buttonDownListener(xbox_buttons::kB);
	}else{
		buttonUpListener(xbox_buttons::kB);
	}

	// dpad
	if(keyboard->keyDown(dpadUp)){
		buttonDownListener(xbox_buttons::kDPAD_UP);
	}else{
		buttonUpListener(xbox_buttons::kDPAD_UP);
	}if(keyboard->keyDown(dpadDown)){
		buttonDownListener(xbox_buttons::kDPAD_DOWN);
	}else{
		buttonUpListener(xbox_buttons::kDPAD_DOWN);
	}if(keyboard->keyDown(dpadLeft)){
		buttonDownListener(xbox_buttons::kDPAD_LEFT);
	}else{
		buttonUpListener(xbox_buttons::kDPAD_LEFT);
	}if(keyboard->keyDown(dpadRight)){
		buttonDownListener(xbox_buttons::kDPAD_RIGHT);
	}else{
		buttonUpListener(xbox_buttons::kDPAD_RIGHT);
	}
}