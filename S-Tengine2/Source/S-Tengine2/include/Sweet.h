#pragma once

#ifndef FPS
	#define FPS 60
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

#include "VoxMatrices.h"
#include <Configuration.h>
#include "Step.h"

struct GLFWwindow;

namespace sweet{
	extern Configuration config;
	extern Step step;
	extern std::string title;

	/** Equal to glfwGetTime() the last time the delta time correction was calculated */
	extern double lastTimestamp;
	
	/** 
	* Multiply by this to correct for differences in framerate 
	*/
	extern double deltaTimeCorrection;
	
	/**
	* Stores a reference to the main window 
	*/
	extern GLFWwindow * currentContext;

	/**
	* Stores a reference to the global freetype instance
	*/
	extern FT_Library freeTypeLibrary;

	/**
	* Whether AntTweakBar has been initialized or not
	*/
	extern bool antTweakBarInititialized;

	/**
	* Whether AntTweakBar should be drawn by game or not
	*/
	extern bool drawAntTweakBar;
	
	/** 
	* Sets the window properties that need to be initialized before window creation
	*/
	void setGlfwWindowHints();
	
	/** 
	* 
	*/
	GLFWwindow * initWindow();
	/** 
	* 
	*/
	void destructWindow(GLFWwindow * _window);
	
	/**
	* Initializes everyting need to get the engine up and running
	*/
	void initialize(std::string _title);

	/**
	* Calculates the delta time and delta time correction and sets them in step
	*/
	void calculateDeltaTimeCorrection();

	/**
	* Destroys the opengl context, shuts down GLFW and closes the application
	*/
	void destruct();

	/**
	* Intitializes ant tweak bar
	*/
	void initAntTweakBar(GLFWwindow * _context);

	/**
	* Flips drawAntTweakBar 
	* Sets the cursor mode so that ant tweak bar works properly
	*/
	void toggleAntTweakBar();

	void initializeInputCallbacks(GLFWwindow * _context);
	void setGlfwWindowHints();
	void keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods);
	void charCallback(GLFWwindow * _window, unsigned int _key);
	void mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods);
	void mousePositionCallback(GLFWwindow *_window, double _x, double _y);
	void mouseScrollCallback(GLFWwindow *_window, double _x, double _y);
	void attemptToActuallyRegainFocus(GLFWwindow *_window, int _button, int _action, int _mods);
	void windowFocusCallback(GLFWwindow * _window, int _focused);
	void error_callback(int _error, const char * _description);


	/**
	* @returns the width of the current context in pixels 
	*/
	unsigned long int getWindowWidth();
	/**
	* @returns the width of _window in pixels
	*/
	unsigned long int getWindowWidth(GLFWwindow * _window);
	
	/**
	* @returns the height of the current context in pixels 
	*/
	unsigned long int getWindowHeight();
	/**
	* @returns the height of _window in pixels
	*/
	unsigned long int getWindowHeight(GLFWwindow * _window);
	
	/**
	* @returns the width and height of the current context in pixels as a Dimension object 
	*/
	glm::uvec2 getWindowDimensions();
	/**
	* @returns the width and height of _window in pixels as a Dimension object 
	*/
	glm::uvec2 getWindowDimensions(GLFWwindow * _window);
}