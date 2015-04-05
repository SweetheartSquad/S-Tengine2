#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "ResourceManager.h"

/*************************************************************************
*
* The Game class for VOX. There should only ever be one instance of Game.
* This game will hold and update scenes. This Game should be updated
* in the main render loop in the Main class
*
**************************************************************************/
class Step;
class Game : public NodeUpdatable{
private:
	/**
	* Prints the frames per second which this game class
	* is running to the console
	*/
	void printFps();
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
	// Key of the current scene in scenemap
	std::string currentSceneKey;
	/** List of references to scenes in the game */
	std::map<std::string, Scene*> scenes;

	/**Perorms a full game loop*/
	void performGameLoop();

	/** 
	* Updates the deltaTime correction, prints FPS, checks for ESC, and calls update on the current scene 
	*/
	virtual void update(Step * _step) override;
	
	/** 
	* Draws the current scene 
	*/
	virtual void draw(void);

	bool switchingScene;
	std::string newSceneKey;
	bool deleteOldScene;
	void switchScene(std::string _newSceneKey, bool _deleteOldScene);
	
	/** 
	* Calls update on the mouse and keyboard 
	*/
	void manageInput(void);

	/**
	* Toggles the fullscreen mode. Destroys the current glfwContext and makes a new one; sets all scenes to dirty
	*/
	void toggleFullScreen();

	virtual void setViewport(float _x, float _y, float _width, float _height);
	int viewPortX, viewPortY, viewPortWidth, viewPortHeight;
	
	unsigned long int kc_lastKey;
	unsigned long int kc_code;
	bool kc_active;
	bool kc_just_active;
};
