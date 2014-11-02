#pragma once

#ifndef FPS
	#define FPS 60
#endif

#include <string>

#include "VoxMatrices.h"

class GLFWwindow;

namespace vox{
	
	/** Equal to glfwGetTime() the last time the delta time correction was calculated */
	extern double lastTimestamp;
	/** Multiply by this to correct for differences in framerate */
	extern double deltaTimeCorrection;
	/** Stores a reference to the main window */
	extern GLFWwindow * currentContext;
	/** Whether the main window is fullscreen or not */
	extern bool fullscreen;
	/** Sets the window properties that need to be initialized before window creation */
	void setGlfwWindowHints();
	/** Sets the window properties that need to be initialized after window creation (uses currentContext if null) */
	void initWindow(GLFWwindow * _w = nullptr);

	void setWindowName(std::string _name);

	void calculateDeltaTimeCorrection();

	void destruct();
}
