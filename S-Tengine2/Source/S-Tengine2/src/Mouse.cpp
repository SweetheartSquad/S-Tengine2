#pragma once

#include <Mouse.h>

#include <System.h>
#include <algorithm>

Mouse::Mouse():
	x(NULL),
	y(NULL),
	clampedX(NULL),
	clampedY(NULL)
{
}

Mouse::~Mouse(){
}

bool Mouse::leftJustPressed(){
	return justPressedButtons.find(GLFW_MOUSE_BUTTON_LEFT) != justPressedButtons.end();
}

bool Mouse::leftJustReleased(){
	return justReleasedButtons.find(GLFW_MOUSE_BUTTON_LEFT) != justReleasedButtons.end();
}

bool Mouse::leftDown(){
	return pressedButtons.find(GLFW_MOUSE_BUTTON_LEFT) != pressedButtons.end();
}

bool Mouse::rightJustPressed(){
	return justPressedButtons.find(GLFW_MOUSE_BUTTON_RIGHT) != justPressedButtons.end();
}

bool Mouse::rightJustReleased(){
	return justReleasedButtons.find(GLFW_MOUSE_BUTTON_RIGHT) != justReleasedButtons.end();
}

bool Mouse::rightDown(){
	return pressedButtons.find(GLFW_MOUSE_BUTTON_RIGHT) != pressedButtons.end();
}

double Mouse::mouseX(bool _clamped){
	return _clamped ? clampedX : x;
}
void Mouse::mouseX(double _x){
	
}

double Mouse::mouseY(bool _clamped){
	return _clamped ? clampedY : y;
}
void Mouse::mouseY(double _y){
	
}

void Mouse::update(){
	justPressedButtons.clear();
	justReleasedButtons.clear();
}

void Mouse::mouseDownListener(int _glfwMouseCode){
	justPressedButtons.insert(std::make_pair(_glfwMouseCode, _glfwMouseCode));
	pressedButtons.insert(std::make_pair(_glfwMouseCode, _glfwMouseCode));
}

void Mouse::mouseUpListener(int _glfwMouseCode){
	justReleasedButtons.insert(std::make_pair(_glfwMouseCode, _glfwMouseCode));

	if(pressedButtons.find(_glfwMouseCode) != pressedButtons.end()){
		pressedButtons.erase(_glfwMouseCode);
	}

	if(justPressedButtons.find(_glfwMouseCode) != justPressedButtons.end()){
		justPressedButtons.erase(_glfwMouseCode);
	}
}

void Mouse::mousePositionListener(double _x, double _y){
	glm::uvec2 sd = vox::getScreenDimensions();
	clampedX = std::max(0., std::min(clampedX + _x - x, (double)sd.x));
	clampedY = std::max(0., std::min(clampedY + _y - y, (double)sd.y));

	x = _x;
	y = _y;
}

Mouse& Mouse::getInstance(){
	static Mouse *mouse;
	if(mouse == 0){
		mouse = new Mouse();
	}
	return *mouse;
}