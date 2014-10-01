#pragma once

#include <iostream>
#include <map>

// Singleton encapsulating keyboard events and properties
class Keyboard
{
public:
	// Returns true if the given key is currently pressed down
	bool keyDown(int _glfwKeyCode);
	// Returns true if the given key was released since the keyboard's last call to update
	bool keyJustUp(int _glfwKeyCode);
	// Returns true if the given key was pressed down since the keyboard's last call to update
	bool keyJustDown(int _glfwKeyCode);
	// Clears the lists of justPressed and justReleased keys
	void update();
	
	// Inserts _glfwKeyCode into the lists of justPressed and pressed keys
	void keyDownListener(int _glfwKeyCode);
	// Inserts _glfwKeyCode into the list of justPressed keys and removes it from the lists of justPressed and pressed keys
	void keyUpListener(int _glfwKeyCode);
	
	// Returns a reference to the keyboard singleton
	static Keyboard& getInstance();

private:
	Keyboard();
	~Keyboard();
	
	// List of keys which are currently pressed down
	std::map<int, int> pressedKeys;
	// List of keys which were pressed down since the keyboard's last call to update
	std::map<int, int> justPressedKeys;
	// List of keys which were released since the keyboard's last call to update
	std::map<int, int> justReleasedKeys;
};