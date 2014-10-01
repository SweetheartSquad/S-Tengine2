#pragma once

#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vox.h"
#include "RenderSystem.h"
#include "Scene.h"
#include "Entity.h"

class Game{
private:
	void printFps();

public:
	Mouse * mouse;			// Reference to mouse singleton
	Keyboard * keyboard;	// Reference to keyboard singleton

	bool isRunning;			// True if the game is currently running
	bool printFPS;			// Whether to print the fps in the console every second

	GLFWwindow *glfwWindow;		// Reference to the game's window
	RenderSystem *renderSystem;	// Reference to the game's RenderSystem

	std::vector<Entity*> *children;		// Reference to list of references to children (why does the game have children?)
	
	Scene * currentScene;					// Reference to the game's current scene
	std::map<std::string, Scene*> scenes;	// List of references to scenes in the game
	
	explicit Game(bool _running);
	virtual ~Game(void);

	// Updates the deltaTime correction, prints FPS, checks for ESC, and calls update on the current scene
	virtual void update(void);
	// Draws the current scene
	virtual void draw(void);
	// Calls update on the mouse and keyboard
	void manageInput(void);
};
