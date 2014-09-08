#include "Mouse.h"

Mouse::Mouse():
	pressedKeys(new std::map<int, int>()),
	justPressedKeys(new std::map<int, int>()),
	justReleasedKeys(new std::map<int, int>()),
	x(NULL),
	y(NULL)
{
}

Mouse::~Mouse(){
	delete justPressedKeys;
	delete justReleasedKeys;
	delete pressedKeys;
}

bool Mouse::leftJustPressed(){
	return justPressedKeys->find(GLFW_MOUSE_BUTTON_LEFT) != justPressedKeys->end();
}

bool Mouse::lefJustReleased(){
	return justReleasedKeys->find(GLFW_MOUSE_BUTTON_LEFT) != justReleasedKeys->end();
}

bool Mouse::leftDown(){
	return pressedKeys->find(GLFW_MOUSE_BUTTON_LEFT) != pressedKeys->end();
}

bool Mouse::rightJustPressed(){
	return justPressedKeys->find(GLFW_MOUSE_BUTTON_RIGHT) != justPressedKeys->end();
}

bool Mouse::rightJustReleased(){
	return justReleasedKeys->find(GLFW_MOUSE_BUTTON_RIGHT) != justReleasedKeys->end();
}

bool Mouse::rightDown(){
	return pressedKeys->find(GLFW_MOUSE_BUTTON_RIGHT) != pressedKeys->end();
}

double Mouse::mouseX(){
	return x;
}

double Mouse::mouseY(){
	return y;
}

void Mouse::update(){
	justPressedKeys->clear();
	justReleasedKeys->clear();
}

void Mouse::mouseDownListener(int _glfwMouseCode){
	justPressedKeys->insert(std::make_pair(_glfwMouseCode, _glfwMouseCode));
	pressedKeys->insert(std::make_pair(_glfwMouseCode, _glfwMouseCode));
}

void Mouse::mouseUpListener(int _glfwMouseCode){
	justReleasedKeys->insert(std::make_pair(_glfwMouseCode, _glfwMouseCode));
	
	if(pressedKeys->find(_glfwMouseCode) != pressedKeys->end()){
		pressedKeys->erase(_glfwMouseCode);
	}
	
	if(justPressedKeys->find(_glfwMouseCode) != justPressedKeys->end()){
		justPressedKeys->erase(_glfwMouseCode);
	}
}

void Mouse::mousePositionListener(double _x, double _y){
	this->x = _x;
	this->y = _y;
}

void Mouse::setMousePositionListener(double _x, double _y){
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

