#pragma once

#ifndef FPS
#define FPS 60
#endif

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace vox{

extern std::vector<glm::mat4> *matrixStack;
extern glm::mat4 currentModelMatrix;
extern double lastTimestamp;
extern double deltaTimeCorrection;

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
