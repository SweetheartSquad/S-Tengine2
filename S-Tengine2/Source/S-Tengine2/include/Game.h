#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "ResourceManager.h"

#define VOX_LIMIT_FRAMERATE 1

/*************************************************************************
*
* The Game class for VOX. There should only ever be one instance of Game.
* This game will hold and update scenes. This Game should be updated
* in the main render loop in the Main class
*
**************************************************************************/
class Step;
class Scene_Splash;
class Game : public virtual NodeUpdatable, public virtual NodeLoadable{
protected:
	// adds the given scene to the splash scene queue
	// and points the previous splash scene to it
	void addSplash(Scene_Splash * _splashScene);
	// called in Game's constructor
	// should be overridden to contain calls to addSplash
	// in order to add splash scenes to a game before the
	// initial scene is loaded
	virtual void addSplashes();
private:
	/**
	* Prints the frames per second which this game class
	* is running to the console
	*/
	void printFps();
	
	double accumulator;
	double lastTimestep;

	double lastTime;
	int nbFrames;

	unsigned long int numSplashScenes;
	
	
public:
	// must be called in the main before the game loop is started
	void init();

	bool splashScreen;

	explicit Game(std::pair<std::string, Scene *> _firstScene, bool _running = true);
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
	* Toggles the fullscreen mode. Unloads, destroys the current glfwContext, makes a new glfwContext, and reloads
	* sets all scenes to dirty
	*/
	void toggleFullScreen();

	virtual void load() override;
	virtual void unload() override;


	// saves the current viewport with the format "../screenshots/YYYY-MM-DD_TTTTTTTTTT.tga"
	void takeScreenshot();

	// flags the current glfw context as "should close" so that the game will quit
	void exit();

	// whether the game will call resize on draw and fullscreen calls
	bool autoResize;
	// sets the viewport to match the current window size
	void resize();

	virtual void setViewport(float _x, float _y, float _width, float _height);
	int viewPortX, viewPortY, viewPortWidth, viewPortHeight;
	
	unsigned long int kc_lastKey;
	unsigned long int kc_code;
	bool kc_active;
	bool kc_just_active;
};
