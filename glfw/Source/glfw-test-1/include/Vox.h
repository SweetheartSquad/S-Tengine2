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

void calculateModelMatrix();
void pushMatrix(glm::mat4 modelMatrix);
void popMatrix();
glm::mat4 modelMatrix();
void clearMatrixStack();

void calculateDeltaTimeCorrection();

void destruct();

}
