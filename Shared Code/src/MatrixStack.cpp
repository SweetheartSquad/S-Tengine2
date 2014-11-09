#pragma once

#include "MatrixStack.h"

MatrixStack::MatrixStack():
	currentModelMatrix(glm::mat4(1)),
	projectionMatrix(glm::mat4(1)),
	viewMatrix(glm::mat4(1)){
}

MatrixStack::~MatrixStack(){
}

void MatrixStack::popMatrix(){
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

void MatrixStack::pushMatrix(){
	matrixStack.push_back(currentModelMatrix);
}

glm::mat4 MatrixStack::getCurrentMatrix(){
	return currentModelMatrix;
}

void MatrixStack::scale(glm::mat4 _scaleMatrix){
	currentModelMatrix = currentModelMatrix * _scaleMatrix;
}

void MatrixStack::rotate(glm::mat4 _rotationMatrix){
	currentModelMatrix = currentModelMatrix * _rotationMatrix;
}

void MatrixStack::translate(glm::mat4 _translationMatrix){
	currentModelMatrix = currentModelMatrix * _translationMatrix;
}

void MatrixStack::applyMatrix(glm::mat4 _modelMatrix){
	currentModelMatrix = currentModelMatrix * _modelMatrix;
}

void MatrixStack::resetCurrentMatrix(){
	currentModelMatrix = glm::mat4(1);
}
void MatrixStack::clearMatrixStack(){
	matrixStack.clear();
}