#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Vox.h"

namespace  vox
{
	
static std::vector<glm::mat4> *matrixStack;
static glm::mat4 currentModelMatrix;

static void calculateModelMatrix()
{
	if(!matrixStack){
		matrixStack = new std::vector<glm::mat4>();
	}

	glm::mat4 returnMatrix = glm::mat4(1);
	for(glm::mat4 m : *matrixStack){
		returnMatrix = returnMatrix * m;
	}
	currentModelMatrix = returnMatrix;
}

static void pushMatrix(glm::mat4 modelMatrix)
{
	if(!matrixStack){
		matrixStack = new std::vector<glm::mat4>();
	}
	matrixStack->push_back(modelMatrix);
	calculateModelMatrix();
}

static void popMatrix()
{
	if(!matrixStack){
		matrixStack = new std::vector<glm::mat4>();
	}
	if(matrixStack->size() > 0){
		matrixStack->pop_back();	
		calculateModelMatrix();
	}
}

static glm::mat4 modelMatrix(){
	return currentModelMatrix;
}

static void clearMatrixStack(){
	matrixStack->clear();
}

static void destruct(){
	delete matrixStack;
}

}
