#pragma once

#include <GLFW/glfw3.h>

#include "Sweet.h"

/**********************************************
*
* A collection of system utility functions
*
***********************************************/
namespace sweet{
	/**
	* @returns the width of the current context in pixels 
	*/
	unsigned long int getScreenWidth();
	
	/**
	* @returns the height of the current context in pixels 
	*/
	unsigned long int getScreenHeight();
	
	/**
	* @returns the width and height of the current context in pixels as a Dimension object 
	*/
	glm::uvec2 getScreenDimensions();
};
