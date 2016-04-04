#pragma once

#include <Mouse.h>
#include <Sweet.h>
#include <algorithm>

#include <GLFW/glfw3.h>

Mouse::Mouse():
	x(NULL),
	y(NULL),
	clampedX(NULL),
	clampedY(NULL),
	active(true),
	mouseWheelDelta(0)
{
}

Mouse::~Mouse(){
}

bool Mouse::leftJustPressed() const{
	return justPressedButtons.find(GLFW_MOUSE_BUTTON_LEFT) != justPressedButtons.end();
}

bool Mouse::leftJustReleased() const{
	return justReleasedButtons.find(GLFW_MOUSE_BUTTON_LEFT) != justReleasedButtons.end();
}

bool Mouse::leftDown() const{
	return pressedButtons.find(GLFW_MOUSE_BUTTON_LEFT) != pressedButtons.end();
}

bool Mouse::rightJustPressed() const{
	return justPressedButtons.find(GLFW_MOUSE_BUTTON_RIGHT) != justPressedButtons.end();
}

bool Mouse::rightJustReleased() const{
	return justReleasedButtons.find(GLFW_MOUSE_BUTTON_RIGHT) != justReleasedButtons.end();
}

bool Mouse::rightDown() const{
	return pressedButtons.find(GLFW_MOUSE_BUTTON_RIGHT) != pressedButtons.end();
}

double Mouse::mouseX(bool _clamped) const{
	return _clamped ? clampedX : x;
}

double Mouse::mouseY(bool _clamped) const{
	return _clamped ? clampedY : y;
}

double Mouse::getMouseWheelDelta() const{
	return mouseWheelDelta;
}

void Mouse::translate(glm::vec2 _v, bool _relative){
	if(active){
		glm::vec2 targetPos = _v;
		if(_relative){
			targetPos += glm::vec2(x, y);
		}
		glfwSetCursorPos(sweet::currentContext, targetPos.x, sweet::getWindowHeight() - targetPos.y);
		mousePositionListener(targetPos.x, targetPos.y);
	}
}

void Mouse::update(){
	justPressedButtons.clear();
	justReleasedButtons.clear();
	mouseWheelDelta = 0;
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
	if(active){
		glm::uvec2 sd = sweet::getWindowDimensions();

		if(sweet::cursorMode == GLFW_CURSOR_DISABLED){
			clampedX = max(0., min(clampedX + _x - x, (double)sd.x));
			clampedY = max(0., min(clampedY + _y - y, (double)sd.y));
		}

		x = _x;
		y = _y;

		
		if(sweet::cursorMode != GLFW_CURSOR_DISABLED){
			clampedX = max(0., min(x, (double)sd.x));
			clampedY = max(0., min(y, (double)sd.y));
		}
	}
}

void Mouse::mouseWheelListener(double _delta){
	mouseWheelDelta = _delta;
}

Mouse& Mouse::getInstance(){
	static Mouse *mouse;
	if(mouse == nullptr){
		mouse = new Mouse();
	}
	return *mouse;
}