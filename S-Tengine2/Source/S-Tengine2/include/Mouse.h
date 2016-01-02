#pragma once

#include <iostream>
#include <glm\glm.hpp>
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
	// whether the mouse will update its position
	// if you're accessing this, you should probably think twice about what you're doing
	bool active;

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
	* Returns the mouse's current X coordinate. If _clamped, the value will always be within the screen coordinates
	* Relative to the left border
	*
	* @return The mouse's X coordinate
	*/
	double mouseX(bool _clamped = true);

	/**
	* Returns the mouse's current Y coordinate. If _clamped, the value will always be within the screen coordinates
	* Relative to the bottom border
	*
	* @return The mouse's Y coordinate
	*/
	double mouseY(bool _clamped = true);
	
	/*
	* @return The change in mousewheel since the last update
	*/
	double getMouseWheelDelta();
	
	/**
	* Moves the mouse by _v
	* Sets the glfw cursor position and triggers the position listener
	*/
	void translate(glm::vec2 _v);

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
	* Sets the current mouse coordinates to _x and _y, where (0, 0) is the bottom-left corner
	*
	* @param _x The X coordinate for the mouse
	* @param _y The Y coordinate for the mouse
	*/
	void mousePositionListener(double _x, double _y);
	
	/**
	* Saves _delta in a member variable until the next update
	*
	* @param _delta The change in the mousewheel
	*/
	void mouseWheelListener(double _delta);

	/**
	* @return A reference to the mouse singleton
	*/
	static Mouse& getInstance();

private:

	/**
	* Initializes the coordinates of the mouse to (0, 0), the bottom-left corner
	*/
	Mouse();
	~Mouse();

	/** Map of mouse buttons which are currently pressed down */
	std::map<int, int> pressedButtons;
	/** Map of mouse buttons which were pressed down since the mouse's last call to update */
	std::map<int, int> justPressedButtons;
	/** Map of mouse buttons which were released since the mouse's last call to update */
	std::map<int, int> justReleasedButtons;

	/** The mouse's current X coordinate, relative to the left border */
	double x;
	/** The mouse's current Y coordinate, relative to the bottom border */
	double y;

	// The mousewheel's change since the last update
	double mouseWheelDelta;
	
	// The mouse's current X coordinate, relative to the left border and clamped by the screen coordinates
	double clampedX;
	// The mouse's current Y coordinate, relative to the bottom border and clamped by the screen coordinates
	double clampedY;
};