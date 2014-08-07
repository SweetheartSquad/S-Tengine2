#pragma once 

#include <iostream>
#include <GLFW/glfw3.h>
#include <map>

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool isKeyDown(int glfwKeyCode);
	bool keyJustUp(int glfwKeyCode);
	void update();

	void keyDownListener(int glfwKeyCode);
	void keyUpListener(int glfwKeyCode);

	static Keyboard& getInstance();

private:

	std::map<int, int> *pressedKeys;
	std::map<int, int> *justPressedKeys;
	std::map<int, int> *justReleasedKeys;
};