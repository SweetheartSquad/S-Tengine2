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

bool Keyboard::keyJustDown(int glfwKeyCode)
{
	return justPressedKeys->find(glfwKeyCode)!=justPressedKeys->end();
}

void Keyboard::keyDownListener(int glfwKeyCode)
{
	justPressedKeys->insert(std::make_pair(glfwKeyCode, glfwKeyCode));
	pressedKeys->insert(std::make_pair(glfwKeyCode, glfwKeyCode));
}

void Keyboard::keyUpListener(int glfwKeyCode)
{
	justReleasedKeys->insert(std::make_pair(glfwKeyCode, glfwKeyCode));

	if(pressedKeys->find(glfwKeyCode)!=pressedKeys->end())
	{
		pressedKeys->erase(glfwKeyCode);
	}
}

void Keyboard::update()
{
	justPressedKeys->clear();
	justReleasedKeys->clear();
}

Keyboard& Keyboard::getInstance()
{
	static Keyboard *keyboard;
	if(keyboard == 0){
		keyboard= new Keyboard();
	}
	return *keyboard;
}

bool Keyboard::keyDown(int glfwKeyCode)
{
	return justPressedKeys->find(glfwKeyCode) != justPressedKeys->end() || pressedKeys->find(glfwKeyCode) != pressedKeys->end();
}