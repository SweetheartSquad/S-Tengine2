#pragma once

#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"

#include <ft2build.h>
#include FT_FREETYPE_H

/*************************************************************************
*
* The Game class for VOX. There should only ever be one instance of Game.
* This game will hold and update scenes. This Game should be updated
* in the main render loop in the Main class
*
**************************************************************************/

class Game{
private:
	/**
	* Prints the frames per second which this game class
	* is running to the console
	*/
	void printFps();
	FT_Library freetypeLibrary;

public:

	explicit Game(bool _running);
	virtual ~Game(void);

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

	/** Reference to the game's current scene */
	Scene * currentScene;
	/** List of references to scenes in the game */
	std::map<std::string, Scene*> scenes;

	/** 
	* Updates the deltaTime correction, prints FPS, checks for ESC, and calls update on the current scene 
	*/
	virtual void update(void);
	
	/** 
	* Draws the current scene 
	*/
	virtual void draw(void);
	
	/** 
	* Calls update on the mouse and keyboard 
	*/
	void manageInput(void);
	
	/** 
	* Returns a new reference to the game's freetype library instance
	* TODO Actually implement this
	*/
	FT_Library * getFreetypeLibraryReference();
};
