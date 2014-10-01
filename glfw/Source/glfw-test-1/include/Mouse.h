#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <map>

// Singleton encapsulating mouse events and properties
class Mouse{
public:
	// Returns true if the left mouse button was pressed down since the mouse's last call to update
	bool leftJustPressed();
	// Returns true if the left mouse button was released since the mouse's last call to update
	bool leftJustReleased();
	// Returns true if the left mouse button is currently pressed down
	bool leftDown();
	// Returns true if the right mouse button was pressed down since the mouse's last call to update
	bool rightJustPressed();
	// Returns true if the right mouse button was released since the mouse's last call to update
	bool rightJustReleased();
	// Returns true if the right mouse button is currently pressed down
	bool rightDown();
	// Returns the mouse's current X coordinate
	double mouseX();
	// Returns the mouse's current Y coordinate
	double mouseY();
	
	// Clears the maps of justPressed and justReleased buttons
	void update();

	// Inserts _glfwMouseCode into the maps of justPressed and pressed buttons
	void mouseDownListener(int _glfwMouseCode);
	// Inserts _glfwMouseCode into the map of justPressed buttons and removes it from the maps of justPressed and pressed buttons
	void mouseUpListener(int _glfwMouseCode);
	// Sets the current mouse coordinates to _x and _y
	void mousePositionListener(double _x, double _y);

	// Returns a reference to the mouse singleton
	static Mouse& getInstance();

private:
	// Initializes the coordinates of the mouse to (0, 0)
	Mouse();
	~Mouse();
	
	std::map<int, int> pressedButtons;		// Map of mouse buttons which are currently pressed down
	std::map<int, int> justPressedButtons;	// Map of mouse buttons which were pressed down since the mouse's last call to update
	std::map<int, int> justReleasedButtons;	// Map of mouse buttons which were released since the mouse's last call to update
	
	double x;	// The mouse's current X coordinate
	double y;	// The mouse's current Y coordinate
};