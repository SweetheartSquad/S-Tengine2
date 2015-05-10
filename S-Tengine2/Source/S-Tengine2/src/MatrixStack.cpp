#pragma once

#include "MatrixStack.h"

vox::MatrixStack::MatrixStack() :
	currentModelMatrix(glm::mat4(1)),
	projectionMatrix(glm::mat4(1)),
	viewMatrix(glm::mat4(1)),
	mvp(glm::mat4(1)),
	mvpDirty(false),
	vp(glm::mat4(1)),
	vpDirty(false)
{
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
	mvpDirty = true;
}

void vox::MatrixStack::pushMatrix(){
	matrixStack.push_back(currentModelMatrix);
}

const glm::mat4 * vox::MatrixStack::getModelMatrix(){
	return &currentModelMatrix;
}
const glm::mat4 * vox::MatrixStack::getViewMatrix(){
	return &viewMatrix;
}
const glm::mat4 * vox::MatrixStack::getProjectionMatrix(){
	return &projectionMatrix;
}

void vox::MatrixStack::setProjectionMatrix(const glm::mat4 * _projectionMatrix){
	projectionMatrix = *_projectionMatrix;
	vpDirty = true;
	mvpDirty = true;
}
void vox::MatrixStack::setViewMatrix(const glm::mat4 * _viewMatrix){
	viewMatrix = *_viewMatrix;
	vpDirty = true;
	mvpDirty = true;
}

void vox::MatrixStack::scale(glm::mat4 _scaleMatrix){
	currentModelMatrix = currentModelMatrix * _scaleMatrix;
	mvpDirty = true;
}

void vox::MatrixStack::rotate(glm::mat4 _rotationMatrix){
	currentModelMatrix = currentModelMatrix * _rotationMatrix;
	mvpDirty = true;
}

void vox::MatrixStack::translate(glm::mat4 _translationMatrix){
	currentModelMatrix = currentModelMatrix * _translationMatrix;
	mvpDirty = true;
}

void vox::MatrixStack::applyMatrix(glm::mat4 _modelMatrix){
	currentModelMatrix = currentModelMatrix * _modelMatrix;
	mvpDirty = true;
}

void vox::MatrixStack::resetCurrentMatrix(){
	currentModelMatrix = glm::mat4(1);
	mvpDirty = true;
}
void vox::MatrixStack::clearMatrixStack(){
	matrixStack.clear();
}

const glm::mat4 * vox::MatrixStack::getVP(){
	if(vpDirty){
		vp = projectionMatrix * viewMatrix;
		vpDirty = false;
	}
	return &vp;
}

const glm::mat4 * vox::MatrixStack::getMVP(){
	if(mvpDirty){
		mvp = *getVP() * currentModelMatrix;
		mvpDirty = false;
	}
	return &mvp;
}