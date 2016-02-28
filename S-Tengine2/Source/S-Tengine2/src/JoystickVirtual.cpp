#pragma once

#include <JoystickVirtual.h>
#include <GLFW\glfw3.h>

JoystickVirtual::JoystickVirtual(int _id) :
	Joystick(_id),
	keyboard(&Keyboard::getInstance())
{
	name = "Virtual Joystick";
	initVirtual();
}

void JoystickVirtual::initVirtual(){
	// axes/trigger map positions
	axisLeftX = 0;
	axisLeftY = 1;
	axisTriggers = 2;
	axisRightY = 3;
	axisRightX = 4;

	// left axis controls
	axisLeftUp = GLFW_KEY_W;
	axisLeftDown = GLFW_KEY_S;
	axisLeftLeft = GLFW_KEY_A;
	axisLeftRight = GLFW_KEY_D;

	// dpad controls/map positions
	dpadUp = GLFW_KEY_W;
	dpadDown = GLFW_KEY_S;
	dpadLeft = GLFW_KEY_A;
	dpadRight = GLFW_KEY_D;

	// right axis controls
	axisRightUp = GLFW_KEY_I;
	axisRightDown = GLFW_KEY_K;
	axisRightLeft = GLFW_KEY_J;
	axisRightRight = GLFW_KEY_L;

	// bumper controls/map positions
	bumperLeft = GLFW_KEY_Q;
	bumperRight = GLFW_KEY_E;

	// trigger controls
	axisTriggerLeft = GLFW_KEY_Z;
	axisTriggerRight = GLFW_KEY_C;

	// center button controls/map positions
	centerButtonLeft = GLFW_KEY_SPACE;
	centerButtonRight = GLFW_KEY_ESCAPE;

	// face button controls/map positions
	faceButtonUp = GLFW_KEY_KP_8;
	faceButtonDown = GLFW_KEY_KP_2;
	faceButtonLeft = GLFW_KEY_KP_4;
	faceButtonRight = GLFW_KEY_KP_6;
}

void JoystickVirtual::update(Step * _step){
	// left stick
	axesValues[axisLeftY] = 0.f;
	if(keyboard->keyDown(axisLeftUp)){
		axesValues[axisLeftY] -= 1.f;
	}if(keyboard->keyDown(axisLeftDown)){
		axesValues[axisLeftY] += 1.f;
	}
	
	axesValues[axisLeftX] = 0.f;
	if(keyboard->keyDown(axisLeftLeft)){
		axesValues[axisLeftX] -= 1.f;
	}if(keyboard->keyDown(axisLeftRight)){
		axesValues[axisLeftX] += 1.f;
	}

	// right stick
	axesValues[axisRightY] = 0.f;
	if(keyboard->keyDown(axisRightUp)){
		axesValues[axisRightY] -= 1.f;
	}if(keyboard->keyDown(axisRightDown)){
		axesValues[axisRightY] += 1.f;
	}
	
	axesValues[axisRightX] = 0.f;
	if(keyboard->keyDown(axisRightLeft)){
		axesValues[axisRightX] -= 1.f;
	}if(keyboard->keyDown(axisRightRight)){
		axesValues[axisRightX] += 1.f;
	}
	
	// triggers
	axesValues[axisTriggers] = 0.f;
	if(keyboard->keyDown(axisTriggerLeft)){
		axesValues[axisTriggers] -= 1.f;
	}if(keyboard->keyDown(axisTriggerRight)){
		axesValues[axisTriggers] += 1.f;
	}

	// bumpers
	handleButton(bumperLeft);
	handleButton(bumperRight);

	// face buttons
	handleButton(faceButtonUp);
	handleButton(faceButtonDown);
	handleButton(faceButtonLeft);
	handleButton(faceButtonRight);

	// center buttons
	handleButton(centerButtonLeft);
	handleButton(centerButtonRight);

	// dpad
	handleButton(dpadUp);
	handleButton(dpadDown);
	handleButton(dpadLeft);
	handleButton(dpadRight);
}

void JoystickVirtual::handleButton(int _glfwKeyCode){
	if(keyboard->keyDown(_glfwKeyCode)){
		buttonDownListener(_glfwKeyCode);
	}else{
		buttonUpListener(_glfwKeyCode);
	}
}