#pragma once

#include <Joystick.h>

#include <GLFW\glfw3.h>

Joystick::Joystick(int _id, float _deadZone) : 
	id(_id),
	name("No controller found"),
	deadZone(_deadZone)
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
	if(justReleasedButtons.find(_glfwKeyCode) != justReleasedButtons.end()){
		justReleasedButtons.erase(_glfwKeyCode);
	}

	if(pressedButtons.find(_glfwKeyCode) == pressedButtons.end()){
		justPressedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));
	}else if(justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end()){
		justPressedButtons.erase(_glfwKeyCode);
	}
	pressedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));
}

void Joystick::buttonUpListener(int _glfwKeyCode){
	if(justReleasedButtons.find(_glfwKeyCode) != justReleasedButtons.end()){
		justReleasedButtons.erase(_glfwKeyCode);
	}

	if(pressedButtons.find(_glfwKeyCode) != pressedButtons.end()){
		pressedButtons.erase(_glfwKeyCode);
		justReleasedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));
	}
	if(justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end()){
		justPressedButtons.erase(_glfwKeyCode);
	}
}

void Joystick::buttonNullListener(int _glfwKeyCode){
	if(pressedButtons.find(_glfwKeyCode) != pressedButtons.end()){
		pressedButtons.erase(_glfwKeyCode);
	}
	if(justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end()){
		justPressedButtons.erase(_glfwKeyCode);
	}
	if(justReleasedButtons.find(_glfwKeyCode) != justReleasedButtons.end()){
		justReleasedButtons.erase(_glfwKeyCode);
	}
}

float Joystick::getAxis(int _code){
	auto res = axesValues.find(_code);
	return (res != axesValues.end() && std::abs(res->second) > deadZone) ? res->second : 0;
}

void Joystick::update(Step * _step){
	int buttonCount;
	const unsigned char * buttons = glfwGetJoystickButtons(id, &buttonCount);

	//justPressedButtons.clear();
	//justReleasedButtons.clear();
	axesValues.clear();

	for(signed long int i = 0; i < buttonCount; ++i){
		if(buttons[i] == GLFW_PRESS){
			buttonDownListener(i);
		if(i == xbox_buttons::kL1){
			std::cout << "PRESS" << std::endl;
		}
		}else if(buttons[i] == GLFW_RELEASE){
			buttonUpListener(i);
		if(i == xbox_buttons::kL1){
			std::cout << "RELEASE" << std::endl;
		}
		}else{
			buttonNullListener(i);
		if(i == xbox_buttons::kL1){
			std::cout << "NULL" << std::endl;
		}
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