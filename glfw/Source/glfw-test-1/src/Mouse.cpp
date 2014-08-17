#include "Mouse.h"

Mouse::Mouse()
{
	pressedKeys = new std::map<int, int>();
	justPressedKeys = new std::map<int, int>();
	justReleasedKeys = new std::map<int, int>();
}

Mouse::~Mouse()
{
	delete justPressedKeys;
	delete justReleasedKeys;
	delete pressedKeys;
}

bool Mouse::leftJustPressed()
{
	return justPressedKeys->find(GLFW_MOUSE_BUTTON_LEFT) != justPressedKeys->end();
}

bool Mouse::lefJustReleased()
{
	return justReleasedKeys->find(GLFW_MOUSE_BUTTON_LEFT) != justReleasedKeys->end();
}

bool Mouse::leftDown()
{
	return pressedKeys->find(GLFW_MOUSE_BUTTON_LEFT) != pressedKeys->end();
}

bool Mouse::rightJustPressed()
{
	return justPressedKeys->find(GLFW_MOUSE_BUTTON_RIGHT) != justPressedKeys->end();
}

bool Mouse::rightJustReleased()
{
	return justReleasedKeys->find(GLFW_MOUSE_BUTTON_RIGHT) != justReleasedKeys->end();
}

bool Mouse::rightDown()
{
	return pressedKeys->find(GLFW_MOUSE_BUTTON_RIGHT) != pressedKeys->end();
}

double Mouse::mouseX()
{
	return x;
}

double Mouse::mouseY()
{
	return y;
}

void Mouse::update()
{
	justPressedKeys->clear();
	justReleasedKeys->clear();
}

void Mouse::mouseDownListener(int glfwMouseCode)
{
	justPressedKeys->insert(std::make_pair(glfwMouseCode, glfwMouseCode));
	pressedKeys->insert(std::make_pair(glfwMouseCode, glfwMouseCode));
}

void Mouse::mouseUpListener(int glfwMouseCode)
{
	justReleasedKeys->insert(std::make_pair(glfwMouseCode, glfwMouseCode));
	
	if(pressedKeys->find(glfwMouseCode)!=pressedKeys->end())
	{
		pressedKeys->erase(glfwMouseCode);
	}
	
	if(justPressedKeys->find(glfwMouseCode)!=justPressedKeys->end())
	{
		justPressedKeys->erase(glfwMouseCode);
	}
}

void Mouse::mousePositionListener(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Mouse::setMousePositionListener(double x, double y)
{
	this->x = x;
	this->y = y;
}


Mouse& Mouse::getInstance()
{
	static Mouse *mouse;
	if(mouse == 0){
		mouse = new Mouse();
	}
	return *mouse;
}

