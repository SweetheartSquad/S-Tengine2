#pragma once

#include "MatrixStack.h"

vox::MatrixStack::MatrixStack():
	currentModelMatrix(glm::mat4(1)),
	projectionMatrix(glm::mat4(1)),
	viewMatrix(glm::mat4(1)){
}

vox::MatrixStack::~MatrixStack(){
}

void vox::MatrixStack::popMatrix(){
	if(matrixStack.size() > 0){
		currentModelMatrix = matrixStack.at(matrixStack.size() - 1);
		matrixStack.pop_back();
	}else{
		currentModelMatrix = glm::mat4(1);
		throw;
	}
}

void vox::MatrixStack::pushMatrix(){
	matrixStack.push_back(currentModelMatrix);
}

glm::mat4 vox::MatrixStack::getCurrentMatrix(){
	return currentModelMatrix;
}

void vox::MatrixStack::scale(glm::mat4 _scaleMatrix){
	currentModelMatrix = currentModelMatrix * _scaleMatrix;
}

void vox::MatrixStack::rotate(glm::mat4 _rotationMatrix){
	currentModelMatrix = currentModelMatrix * _rotationMatrix;
}

void vox::MatrixStack::translate(glm::mat4 _translationMatrix){
	currentModelMatrix = currentModelMatrix * _translationMatrix;
}

void vox::MatrixStack::applyMatrix(glm::mat4 _modelMatrix){
	currentModelMatrix = currentModelMatrix * _modelMatrix;
}

void vox::MatrixStack::resetCurrentMatrix(){
	currentModelMatrix = glm::mat4(1);
}
void vox::MatrixStack::clearMatrixStack(){
	matrixStack.clear();
}