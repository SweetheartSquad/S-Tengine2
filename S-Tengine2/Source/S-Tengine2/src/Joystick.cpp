#pragma once

#include <Joystick.h>

#include <sstream>
#include <GLFW\glfw3.h>

#include <Log.h>

Joystick::Joystick(int _id, float _deadZone) : 
	id(_id),
	name("No controller found"),
	deadZone(_deadZone)
{
	int present = glfwJoystickPresent(id);
	if(present){
		name = glfwGetJoystickName(id);
		
		// we'll need to test a bunch of different devices to bin these properly
		// only the xbox one has actually been checked
		if (name.compare("Wireless Controller") == 0){
			// PS4
			initPlaystation();
		}else if (name.compare("PLAYSTATION(R)3 Controller") == 0){
			// PS3
			initPlaystation();
		}else if (name.compare("Microsoft PC-joystick driver") == 0){
			// XBOX
			initXbox();
		}else{
			// GENERIC
			initXbox();
		}
		std::stringstream ss;
		ss << "Controller connected to slot " << id << ": " << name;
		Log::info(ss.str());
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
	// if the button was just released last frame, remove it from the justReleased list
	if(justReleasedButtons.find(_glfwKeyCode) != justReleasedButtons.end()){
		justReleasedButtons.erase(_glfwKeyCode);
	}

	// if the button was NOT down last frame, put it in the justPressed list
	// if the button was just down last frame, remove it from the justPressed list
	if(pressedButtons.find(_glfwKeyCode) == pressedButtons.end()){
		justPressedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));
	}else if(justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end()){
		justPressedButtons.erase(_glfwKeyCode);
	}
	pressedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));
}

void Joystick::buttonUpListener(int _glfwKeyCode){
	// if the button was just released last frame, remove it from the justReleased list
	if(justReleasedButtons.find(_glfwKeyCode) != justReleasedButtons.end()){
		justReleasedButtons.erase(_glfwKeyCode);
	}

	// if the button was down last frame, remove it from the pressed and justPressed lists and put it in the justReleased list
	if(pressedButtons.find(_glfwKeyCode) != pressedButtons.end()){
		pressedButtons.erase(_glfwKeyCode);
		if(justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end()){
			justPressedButtons.erase(_glfwKeyCode);
		}
		justReleasedButtons.insert(std::pair<int, int>(_glfwKeyCode, _glfwKeyCode));
	}
}

void Joystick::buttonNullListener(int _glfwKeyCode){
	// if the button isn't doing anything, remove it from all three lists
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

bool Joystick::axisJustAbove(int _code, float _value){
	auto last = axesValuesPrev.find(_code);
	auto now = axesValues.find(_code);

	// if either value doesn't exist, just warn and return false
	if(last == axesValuesPrev.end() || now == axesValues.end()){
		Log::warn("Axis value doesn't exist");
		return false;
	}

	return last->second < _value && now->second > _value;
}

bool Joystick::axisJustBelow(int _code, float _value){
	auto last = axesValuesPrev.find(_code);
	auto now = axesValues.find(_code);

	// if either value doesn't exist, just warn and return false
	if(last == axesValuesPrev.end() || now == axesValues.end()){
		Log::warn("Axis value doesn't exist");
		return false;
	}

	return last->second > _value && now->second < _value;
}

void Joystick::update(Step * _step){
	// handle buttons
	int buttonCount;
	const unsigned char * buttons = glfwGetJoystickButtons(id, &buttonCount);
	for(signed long int i = 0; i < buttonCount; ++i){
		if(buttons[i] == GLFW_PRESS){
			buttonDownListener(i);
		}else if(buttons[i] == GLFW_RELEASE){
			buttonUpListener(i);
		}else{
			// this part shouldn't actually fire
			buttonNullListener(i);
		}
	}
	
	// handle axes
	axesValuesPrev = axesValues;
	axesValues.clear();
	int axesCount;
	const float * axes = glfwGetJoystickAxes(id, &axesCount);
	for(signed long int i = 0; i < axesCount; ++i){
		axesValues.insert(std::pair<int, float>(i, axes[i]));
	}
}

bool Joystick::buttonDown(int _glfwKeyCode){
	return justPressedButtons.find(_glfwKeyCode) != justPressedButtons.end() || pressedButtons.find(_glfwKeyCode) != pressedButtons.end();
}



void Joystick::initXbox(){
	faceButtonDown = 0;
	faceButtonRight = 1;
	faceButtonLeft = 2;
	faceButtonUp = 3;
	bumperLeft = 4;
	bumperRight = 5;
	centerButtonLeft = 6;
	centerButtonCenter = -1; // doesn't get polled?
	centerButtonRight = 7;
	axisButtonLeft = 8;
	axisButtonRight = 9;
	dpadUp = 10;
	dpadDown = 11;
	dpadLeft = 12;
	dpadRight = 13;

	axisLeftX = 0;
	axisLeftY = 1;
	axisTriggers = 2;
	axisRightY = 3;
	axisRightX = 4;
}

void Joystick::initPlaystation(){
	faceButtonDown = -1;
	faceButtonRight = 0;
	faceButtonLeft = 3;
	faceButtonUp = -1;
	bumperLeft = -1;
	bumperRight = -1;
	centerButtonLeft = -1;
	centerButtonCenter = 1;
	centerButtonRight = 7;
	axisButtonLeft = 4; 
	axisButtonRight = 6;
	dpadUp = 5;
	dpadDown = -1;
	dpadLeft = -1;
	dpadRight = -1;

	axisLeftX = -1;
	axisLeftY = -1;
	axisTriggers = -1;
	axisRightY = -1;
	axisRightX = -1;
}

void Joystick::initSNES(){
	faceButtonDown = 2;
	faceButtonRight = 1;
	faceButtonLeft = 3;
	faceButtonUp = 0;
	bumperLeft = 4;
	bumperRight = 5;
	centerButtonLeft = 8;
	centerButtonCenter = -1;
	centerButtonRight = 9;
	axisButtonLeft = -1; 
	axisButtonRight = -1;
	dpadUp = -1;
	dpadDown = -1;
	dpadLeft = -1;
	dpadRight = -1;

	axisLeftX = 0;
	axisLeftY = 1;
	axisTriggers = -1;
	axisRightY = -1;
	axisRightX = -1;
}