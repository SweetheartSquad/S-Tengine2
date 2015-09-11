#pragma once

#ifndef FPS
	#define FPS 60
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

#include "VoxMatrices.h"

#include "Step.h"

struct GLFWwindow;

namespace sweet{
	
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
	* Whether the main window is fullscreen or not 
	*/
	extern bool fullscreen;
	
	/** 
	* Sets the window properties that need to be initialized before window creation
	*/
	void setGlfwWindowHints();
	
	/** 
	* Sets the window properties that need to be initialized after window creation (uses currentContext if null) 
	*/
	GLFWwindow * initWindow();
	
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


	void setGlfwWindowHints();
	void keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods);
	void mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods);
	void mousePositionCallback(GLFWwindow *_window, double _x, double _y);
	void attemptToActuallyRegainFocus(GLFWwindow *_window, int _button, int _action, int _mods);
	void windowFocusCallback(GLFWwindow * _window, int _focused);
	void error_callback(int _error, const char * _description);
}