#pragma once

#include "Entity.h"
#include "RenderOptions.h"
#include "MatrixStack.h"
#include "node/NodeResource.h"
#include "Sprite.h"

#include "glew\glew.h"

Entity::Entity() :
	childTransform(new Transform())
{
}

Entity::~Entity(void){
	delete childTransform;
	childTransform = nullptr;
}

void Entity::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	childTransform->render(_matrixStack, _renderOptions);
}

void Entity::update(Step * _step){
	childTransform->update(_step);
}

void Entity::unload(){
	childTransform->unload();
	NodeLoadable::unload();
}

void Entity::load(){
	childTransform->load();
	NodeLoadable::load();
}

void Entity::setParent(Transform * _parent){
	NodeChild::setParent(_parent);
	childTransform->setParent(_parent);
}

void Entity::makeCumulativeModelMatrixDirty(){
	NodeChild::makeCumulativeModelMatrixDirty();
	childTransform->makeCumulativeModelMatrixDirty();
}

void Entity::printHierarchy(unsigned long int _startDepth){
	NodeChild::printHierarchy(_startDepth);
	childTransform->printHierarchy(_startDepth);
}