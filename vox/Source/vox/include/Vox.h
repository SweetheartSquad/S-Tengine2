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

namespace vox{
	
	extern Step step;

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
	void initWindow(GLFWwindow * _w = nullptr);
	
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
}
