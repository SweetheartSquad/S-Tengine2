#pragma once

#ifndef FPS
#define FPS 60
#endif

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace vox
{
	
extern std::vector<glm::mat4> *matrixStack;
extern glm::mat4 currentModelMatrix;
extern double lastTimestamp;
extern double deltaTimeCorrection;

extern GLFWwindow * currentContext; //stores a reference to the main window
extern bool fullscreen; //whether the main window is fullscreen or not
void setGlfwWindowHints();

void calculateModelMatrix();
void pushMatrix(glm::mat4 modelMatrix);
void popMatrix();
glm::mat4 modelMatrix();
void clearMatrixStack();

void calculateDeltaTimeCorrection();

void destruct();

}
