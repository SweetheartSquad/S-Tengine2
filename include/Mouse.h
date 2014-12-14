#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <map>

/*******************************************************
*
* Singleton encapsulating mouse events and properties
*
* TODO - Add modifier keys to the value section of the maps
*
********************************************************/

class Mouse{
public:

	/**
	* Returns true if the left mouse button was pressed down since the mouse's last call to update
	*
	* @return Whether the left mouse button was pressed since the last mouse update
	*/
	bool leftJustPressed();

	/**
	* Returns true if the left mouse button was released since the mouse's last call to update
	*
	* @return Whether the left mouse button was released since the last mouse update
	*/
	bool leftJustReleased();

	/**
	* Returns true if the left mouse button is currently pressed down
	*
	* @return Whether the left mouse button is currently pressed down or not
	*/
	bool leftDown();

	/**
	* Returns true if the right mouse button was pressed down since the mouse's last call to update
	*
	* @return Whether the right mouse button was pressed since the last mouse update
	*/
	bool rightJustPressed();

	/**
	* Returns true if the right mouse button was released since the mouse's last call to update
	*
	* @return Whether the right mouse button was released since the last mouse update
	*/
	bool rightJustReleased();

	/**
	* Returns true if the right mouse button is currently pressed down
	*
	* @return Whether the right mouse button is currently pressed or not
	*/
	bool rightDown();

	/**
	* Returns the mouse's current X coordinate
	*
	* @return The mouse's X coordinate
	*/
	double mouseX();

	/**
	* Returns the mouse's current Y coordinate
	*
	* @return The mouse's Y coordinate
	*/
	double mouseY();

	/**
	* Clears the maps of justPressed and justReleased buttons
	*/
	void update();

	/**
	* Inserts _glfwMouseCode into the maps of justPressed and pressed buttons
	*
	* @param _glfwMouseCode The GLFW Mouse Code. eg. GLFW_MOUSE_BUTTON_LEFT
	*/
	void mouseDownListener(int _glfwMouseCode);

	/**
	* Inserts _glfwMouseCode into the map of justPressed buttons and removes it from the maps of justPressed and pressed buttons
	*
	* @param _glfwMouseCode The GLFW Mouse Code. eg. GLFW_MOUSE_BUTTON_LEFT
	*/
	void mouseUpListener(int _glfwMouseCode);

	/**
	* Sets the current mouse coordinates to _x and _y
	*
	* @param _x The X coordinate for the mouse
	* @param _y The Y coordinate for the mouse
	*/
	void mousePositionListener(double _x, double _y);

	/**
	* @return A reference to the mouse singleton
	*/
	static Mouse& getInstance();

private:

	/**
	* Initializes the coordinates of the mouse to (0, 0)
	*/
	Mouse();
	~Mouse();

	/** Map of mouse buttons which are currently pressed down */
	std::map<int, int> pressedButtons;
	/** Map of mouse buttons which were pressed down since the mouse's last call to update */
	std::map<int, int> justPressedButtons;
	/** Map of mouse buttons which were released since the mouse's last call to update */
	std::map<int, int> justReleasedButtons;

	/** The mouse's current X coordinate */
	double x;
	/** The mouse's current Y coordinate */
	double y;
};