#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <map>

class Keyboard
{
public:
	bool keyDown(int _glfwKeyCode);
	bool keyJustUp(int _glfwKeyCode);
	bool keyJustDown(int _glfwKeyCode);
	void update();

	void keyDownListener(int _glfwKeyCode);
	void keyUpListener(int _glfwKeyCode);

	static Keyboard& getInstance();

private:
	Keyboard();
	~Keyboard();

	std::map<int, int> *pressedKeys;
	std::map<int, int> *justPressedKeys;
	std::map<int, int> *justReleasedKeys;
};