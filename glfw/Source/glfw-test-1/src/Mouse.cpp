#include "Mouse.h"

Mouse::Mouse():
	x(NULL),
	y(NULL)
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

double Mouse::mouseX(){
	return x;
}

double Mouse::mouseY(){
	return y;
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
	this->x = _x;
	this->y = _y;
}

Mouse& Mouse::getInstance(){
	static Mouse *mouse;
	if(mouse == 0){
		mouse = new Mouse();
	}
	return *mouse;
}