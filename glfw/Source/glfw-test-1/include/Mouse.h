#pragma once 

#include <iostream>
#include <GLFW/glfw3.h>
#include <map>

class Mouse
{
public:
	
	bool leftJustPressed();
	bool lefJustReleased();
	bool leftDown();
	bool rightJustPressed();
	bool rightJustReleased();
	bool rightDown();
	double mouseX();
	double mouseY();

	void update();

	void mouseDownListener(int glfwMouseCode);
	void mouseUpListener(int glfwMouseCode);
	void mousePositionListener(double x, double y);
	void setMousePositionListener(double x, double y);

	static Mouse& getInstance();

private:
	Mouse();
	~Mouse();

	std::map<int, int> *pressedKeys;
	std::map<int, int> *justPressedKeys;
	std::map<int, int> *justReleasedKeys;

	double x;
	double y;
};