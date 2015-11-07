#pragma once

#include "VoxMatrices.h"

std::vector<glm::mat4> sweet::matrixStack;
glm::mat4 sweet::currentModelMatrix = glm::mat4(1);

/////////// Matrix Stack Begin //////////////
void sweet::popMatrix(){
	if(matrixStack.size() > 0){
		if(matrixStack.size() > 0){
			currentModelMatrix = matrixStack.at(matrixStack.size() - 1);
			matrixStack.pop_back();
		}else{
			currentModelMatrix = glm::mat4(1);
			throw;
		}
	}else{
		currentModelMatrix = glm::mat4(1);
		throw;
	}
}

void sweet::pushMatrix(){
	matrixStack.push_back(currentModelMatrix);
}

glm::mat4 sweet::getCurrentMatrix(){
	return currentModelMatrix;
}

void sweet::scale(glm::mat4 _scaleMatrix){
	currentModelMatrix = currentModelMatrix * _scaleMatrix;
}

void sweet::rotate(glm::mat4 _rotationMatrix){
	currentModelMatrix = currentModelMatrix * _rotationMatrix;
}

void sweet::translate(glm::mat4 _translationMatrix){
	currentModelMatrix = currentModelMatrix * _translationMatrix;
}

void sweet::applyMatrix(glm::mat4 _modelMatrix){
	currentModelMatrix = currentModelMatrix * _modelMatrix;
}

void sweet::resetCurrentMatrix(){
	currentModelMatrix = glm::mat4(1);
}
void sweet::clearMatrixStack(){
	matrixStack.clear();
}