#pragma once

#include <iostream>
#include <map>

// Singleton encapsulating keyboard events and properties
class Keyboard{
public:
	// Returns true if the given key is currently pressed down
	bool keyDown(int _glfwKeyCode);
	// Returns true if the given key was released since the keyboard's last call to update
	bool keyJustUp(int _glfwKeyCode);
	// Returns true if the given key was pressed down since the keyboard's last call to update
	bool keyJustDown(int _glfwKeyCode);
	// Clears the maps of justPressed and justReleased keys
	void update();
	
	// Inserts _glfwKeyCode into the maps of justPressed and pressed keys
	void keyDownListener(int _glfwKeyCode);
	// Inserts _glfwKeyCode into the map of justPressed keys and removes it from the maps of justPressed and pressed keys
	void keyUpListener(int _glfwKeyCode);
	
	// Returns a reference to the keyboard singleton
	static Keyboard& getInstance();

private:
	Keyboard();
	~Keyboard();
	
	std::map<int, int> pressedKeys;			// Map of keys which are currently pressed down
	std::map<int, int> justPressedKeys;		// Map of keys which were pressed down since the keyboard's last call to update
	std::map<int, int> justReleasedKeys;	// Map of keys which were released since the keyboard's last call to update
};