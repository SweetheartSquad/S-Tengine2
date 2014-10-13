#pragma once

#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vox.h"
#include "Scene.h"
#include "Entity.h"

class Game{
private:
	void printFps();

public:
	/** Reference to mouse singleton */
	Mouse * mouse;
	/** Reference to keyboard singleton */
	Keyboard * keyboard;

	/** True if the game is currently running */
	bool isRunning;
	/** Whether to print the fps in the console every second */
	bool printFPS;

	/** Reference to the game's window */
	GLFWwindow *glfwWindow;

	/** Reference to list of references to children (why does the game have children?) */
	std::vector<Entity*> *children;

	/** Reference to the game's current scene */
	Scene * currentScene;
	/** List of references to scenes in the game */
	std::map<std::string, Scene*> scenes;

	explicit Game(bool _running);
	virtual ~Game(void);

	/** Updates the deltaTime correction, prints FPS, checks for ESC, and calls update on the current scene */
	virtual void update(void);
	/** Draws the current scene */
	virtual void draw(void);
	/** Calls update on the mouse and keyboard */
	void manageInput(void);
};
