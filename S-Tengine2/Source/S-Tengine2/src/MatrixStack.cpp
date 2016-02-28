#pragma once

#include "MatrixStack.h"
#include <Camera.h>

sweet::MatrixStack::MatrixStack() :
	currentModelMatrix(glm::mat4(1)),
	projectionMatrix(glm::mat4(1)),
	viewMatrix(glm::mat4(1)),
	mvp(glm::mat4(1)),
	mvpDirty(false),
	vp(glm::mat4(1)),
	vpDirty(false)
{
}

sweet::MatrixStack::~MatrixStack(){
}

void sweet::MatrixStack::popMatrix(){
	if(matrixStack.size() > 0){
		currentModelMatrix = matrixStack.at(matrixStack.size() - 1);
		matrixStack.pop_back();
	}else{
		currentModelMatrix = glm::mat4(1);
		throw;
	}
	mvpDirty = true;
}

void sweet::MatrixStack::pushMatrix(){
	matrixStack.push_back(currentModelMatrix);
}

const glm::mat4 * sweet::MatrixStack::getModelMatrix(){
	return &currentModelMatrix;
}
const glm::mat4 * sweet::MatrixStack::getViewMatrix(){
	return &viewMatrix;
}
const glm::mat4 * sweet::MatrixStack::getProjectionMatrix(){
	return &projectionMatrix;
}

void sweet::MatrixStack::setProjectionMatrix(const glm::mat4 * _projectionMatrix){
	projectionMatrix = *_projectionMatrix;
	vpDirty = true;
	mvpDirty = true;
}
void sweet::MatrixStack::setViewMatrix(const glm::mat4 * _viewMatrix){
	viewMatrix = *_viewMatrix;
	vpDirty = true;
	mvpDirty = true;
}

void sweet::MatrixStack::scale(glm::mat4 _scaleMatrix){
	currentModelMatrix = currentModelMatrix * _scaleMatrix;
	mvpDirty = true;
}

void sweet::MatrixStack::rotate(glm::mat4 _rotationMatrix){
	currentModelMatrix = currentModelMatrix * _rotationMatrix;
	mvpDirty = true;
}

void sweet::MatrixStack::translate(glm::mat4 _translationMatrix){
	currentModelMatrix = currentModelMatrix * _translationMatrix;
	mvpDirty = true;
}

void sweet::MatrixStack::applyMatrix(glm::mat4 _modelMatrix){
	currentModelMatrix = currentModelMatrix * _modelMatrix;
	mvpDirty = true;
}

void sweet::MatrixStack::resetCurrentMatrix(){
	currentModelMatrix = glm::mat4(1);
	mvpDirty = true;
}
void sweet::MatrixStack::clearMatrixStack(){
	matrixStack.clear();
}

const glm::mat4 * sweet::MatrixStack::getVP(){
	if(vpDirty){
		vp = projectionMatrix * viewMatrix;
		vpDirty = false;
	}
	return &vp;
}

const glm::mat4 * sweet::MatrixStack::getMVP(){
	if(mvpDirty){
		mvp = *getVP() * currentModelMatrix;
		mvpDirty = false;
	}
	return &mvp;
}

void sweet::MatrixStack::setCamera(const Camera * _camera){
	const glm::mat4 * p = &_camera->getProjectionMatrix(sweet::getWindowDimensions());
	const glm::mat4 * v = &_camera->getViewMatrix();
	setProjectionMatrix(p);
	setViewMatrix(v);
}