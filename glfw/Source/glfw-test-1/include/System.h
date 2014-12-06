#pragma once

#include <GLFW/glfw3.h>
#include "Dimension.h"

#include "Vox.h"

/**********************************************
*
* A collection of system utility functions
*
***********************************************/
namespace vox{
	/**
	* @returns the width of the current context in pixels 
	*/
	int getScreenWidth();
	
	/**
	* @returns the height of the current context in pixels 
	*/
	int getScreenHeight();
	
	/**
	* @returns the width and height of the current context in pixels as a Dimension object 
	*/
	Dimension getScreenDimensions();
};
