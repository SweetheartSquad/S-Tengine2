#pragma once

#ifndef FPS
#define FPS 60
#endif

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Keyboard.h"
#include "Mouse.h"

namespace vox
{

extern std::vector<glm::mat4> *matrixStack;
extern glm::mat4 currentModelMatrix;
extern double lastTimestamp;
extern double deltaTimeCorrection;

extern GLFWwindow * currentContext; //stores a reference to the main window
extern bool fullscreen; //whether the main window is fullscreen or not

//sets the window properties that need to be initialized before window creation
void setGlfwWindowHints();
//sets the window properties that need to be initialized after window creation (uses currentContext if null)
void initWindow(GLFWwindow * w = nullptr);

void calculateModelMatrix();
void pushMatrix();
void popMatrix();
glm::mat4 getCurrentMatrix();
void clearMatrixStack();

void scale(glm::mat4 scaleMatrix);
void rotate(glm::mat4 rotationMatrix);
void translate(glm::mat4 translationMatrix);
void applyMatrix(glm::mat4 modelMatrix);

void calculateDeltaTimeCorrection();

void destruct();

}
