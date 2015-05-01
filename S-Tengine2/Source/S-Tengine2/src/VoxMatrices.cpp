#pragma once

#include "VoxMatrices.h"

std::vector<glm::mat4> vox::matrixStack;
glm::mat4 vox::currentModelMatrix = glm::mat4(1);

/////////// Matrix Stack Begin //////////////
void vox::popMatrix(){
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

void vox::pushMatrix(){
	matrixStack.push_back(currentModelMatrix);
}

glm::mat4 vox::getCurrentMatrix(){
	return currentModelMatrix;
}

void vox::scale(glm::mat4 _scaleMatrix){
	currentModelMatrix = currentModelMatrix * _scaleMatrix;
}

void vox::rotate(glm::mat4 _rotationMatrix){
	currentModelMatrix = currentModelMatrix * _rotationMatrix;
}

void vox::translate(glm::mat4 _translationMatrix){
	currentModelMatrix = currentModelMatrix * _translationMatrix;
}

void vox::applyMatrix(glm::mat4 _modelMatrix){
	currentModelMatrix = currentModelMatrix * _modelMatrix;
}

void vox::resetCurrentMatrix(){
	currentModelMatrix = glm::mat4(1);
}
void vox::clearMatrixStack(){
	matrixStack.clear();
}