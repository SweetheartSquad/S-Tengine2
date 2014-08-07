#include "Keyboard.h"

Keyboard::Keyboard()
{
	pressedKeys = new std::map<int, int>();
	justPressedKeys = new std::map<int, int>();
	justReleasedKeys = new std::map<int, int>();
}

Keyboard::~Keyboard()
{
	delete pressedKeys;
	delete justPressedKeys;
	delete justReleasedKeys;
}

bool Keyboard::keyJustUp(int glfwKeyCode)
{
	return justReleasedKeys->find(glfwKeyCode)!=justReleasedKeys->end();
}

void Keyboard::keyDownListener(int glfwKeyCode)
{
	justPressedKeys->insert(std::make_pair(glfwKeyCode, glfwKeyCode));
}

void Keyboard::keyUpListener(int glfwKeyCode)
{
	if(justReleasedKeys->find(glfwKeyCode)!=justReleasedKeys->end())
	{
		justReleasedKeys->erase(glfwKeyCode);
	}
}

void Keyboard::update()
{
	
}

Keyboard& Keyboard::getInstance()
{
	static Keyboard *renderSystem;
	if(renderSystem == 0){
		renderSystem = new Keyboard();
	}
	return *renderSystem;
}

bool Keyboard::isKeyDown(int glfwKeyCode)
{
	return justPressedKeys->find(glfwKeyCode) != justPressedKeys->end() || pressedKeys->find(glfwKeyCode) != pressedKeys->end();
}