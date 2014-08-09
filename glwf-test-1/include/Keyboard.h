#pragma once 

#include <iostream>
#include <GLFW/glfw3.h>
#include <map>


class Keyboard
{
public:
	bool keyDown(int glfwKeyCode);
	bool keyJustUp(int glfwKeyCode);
	bool keyJustDown(int glfwKeyCode);
	void update();

	void keyDownListener(int glfwKeyCode);
	void keyUpListener(int glfwKeyCode);

	static Keyboard& getInstance();

private:
	Keyboard();
	~Keyboard();

	std::map<int, int> *pressedKeys;
	std::map<int, int> *justPressedKeys;
	std::map<int, int> *justReleasedKeys;
};