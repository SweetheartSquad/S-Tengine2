#pragma once

#include <Joystick.h>

#include <GLFW\glfw3.h>

Joystick::Joystick(int _id) : 
	id(_id),
	name("No controller found")
{
	int present = glfwJoystickPresent(id);
	if(present){
		name = glfwGetJoystickName(id);
	}
}

Joystick::~Joystick(){
	pressedButtons.clear();
	justPressedButtons.clear();
	justReleasedButtons.clear();
	axesValues.clear();
}

bool Joystick::buttonJustUp(int _glfwKeyCode){
	return justReleasedButtons.find(_glfwKeyCode) != justReleasedButtons.end();
}

bool Joystick::buttonJustDown(int _glfwKeyCode){
	return justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end();
}

void Joystick::buttonDownListener(int _glfwKeyCode){
	justPressedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));
	pressedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));
}

void Joystick::buttonUpListener(int _glfwKeyCode){
	justReleasedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));

	if(pressedButtons.find(_glfwKeyCode) != pressedButtons.end()){
		pressedButtons.erase(_glfwKeyCode);
	}

	if(justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end()){
		justPressedButtons.erase(_glfwKeyCode);
	}
}

float Joystick::getAxis(int _code){
	auto res = axesValues.find(_code);
	if (res != axesValues.end()){
		return res->second;
	}else{
		return 0;
	}
}

void Joystick::update(Step * _step){
	justPressedButtons.clear();
	justReleasedButtons.clear();
	axesValues.clear();

	int buttonCount;
	const unsigned char * buttons = glfwGetJoystickButtons(id, &buttonCount);
	for(signed long int i = 0; i < buttonCount; ++i){
		if(buttons[i] == GLFW_PRESS){
			buttonDownListener(i);
		}else{
			buttonUpListener(i);
		}
	}

	int axesCount;
	const float * axes = glfwGetJoystickAxes(id, &axesCount);
	for(signed long int i = 0; i < axesCount; ++i){
		axesValues.insert(std::pair<int, float>(i, axes[i]));
	}
}

bool Joystick::buttonDown(int _glfwKeyCode){
	return justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end() || pressedButtons.find(_glfwKeyCode) != pressedButtons.end();
}