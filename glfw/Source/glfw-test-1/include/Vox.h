#pragma once

#ifndef FPS
#define FPS 60
#endif

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

//#include <ft2build.h>
//#include FT_FREETYPE_H

#include "Keyboard.h"
#include "Mouse.h"

namespace vox{
	/** Model matrix stack (replaces the OpenGL stack) */
	extern std::vector<glm::mat4> matrixStack;
	/** Current model matrix */
	extern glm::mat4 currentModelMatrix;
	/** Equal to glfwGetTime() the last time the delta time correction was calculated */
	extern double lastTimestamp;
	/** Multiply by this to correct for differences in framerate */
	extern double deltaTimeCorrection;

	/** Stores a reference to the main window */
	extern GLFWwindow * currentContext;
	/** Whether the main window is fullscreen or not */
	extern bool fullscreen;

	// Freetype library
	//extern FT_Library freetypeLibrary;

	/** Sets the window properties that need to be initialized before window creation */
	void setGlfwWindowHints();
	/** Sets the window properties that need to be initialized after window creation (uses currentContext if null) */
	void initWindow(GLFWwindow * _w = nullptr);

	//void calculateModelMatrix();
	/** Pushes the current model matrix onto the stack */
	void pushMatrix();
	/** Restores the current model matrix to the last stored value and pops it off the stack */
	void popMatrix();
	/** Returns the value of the current model matrix */
	glm::mat4 getCurrentMatrix();
	/** Sets the calue of the current model matrix to the identity matrix */
	void resetCurrentMatrix();
	/** Clears the model matrix stack */
	void clearMatrixStack();

	/** currentModelMatrix = currentModelMatrix * _scaleMatrix */
	void scale(			glm::mat4 _scaleMatrix);
	/** currentModelMatrix = currentModelMatrix * _rotationMatrix */
	void rotate(		glm::mat4 _rotationMatrix);
	/** currentModelMatrix = currentModelMatrix * _translationMatrix */
	void translate(		glm::mat4 _translationMatrix);
	/** currentModelMatrix = currentModelMatrix * _modelMatrix */
	void applyMatrix(	glm::mat4 _modelMatrix);

	/** Calculates the value with which to correct for differences in framerate */
	void scale(glm::mat4 _scaleMatrix);
	void rotate(glm::mat4 _rotationMatrix);
	void translate(glm::mat4 _translationMatrix);
	void applyMatrix(glm::mat4 _modelMatrix);

	void setWindowName(std::string _name);

	void calculateDeltaTimeCorrection();

	/** Freetype */
	void initializeFreetype();

	void destruct();
}
