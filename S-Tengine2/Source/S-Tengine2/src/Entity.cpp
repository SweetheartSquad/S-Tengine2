#pragma once

#include <Entity.h>
#include <RenderOptions.h>
#include <MatrixStack.h>
#include <node/NodeResource.h>
#include <Sprite.h>
#include <AntTweakBar.h>

#include <GL\glew.h>

Entity::Entity() :
	childTransform(new Transform()),
	childTransformExists(true)
{
}

Entity::~Entity(void){
	deleteChildTransform();
}

void Entity::deleteChildTransform(){
	if(childTransformExists){
		delete childTransform;
		childTransformExists = false;
	}
}

void Entity::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	// don't bother doing any work if we aren't rendering anyway
	if(!isVisible()){
		return;
	}

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

void Entity::addParent(Transform * _parent){
	NodeChild::addParent(_parent);
	childTransform->addParent(_parent);
}

void Entity::removeParent(Transform * _parent){
	NodeChild::removeParent(_parent);
	childTransform->removeParent(_parent);
}

void Entity::makeCumulativeModelMatrixDirty(){
	NodeChild::makeCumulativeModelMatrixDirty();
	childTransform->makeCumulativeModelMatrixDirty();
}

void Entity::printHierarchy(unsigned long int _startDepth, bool _last, std::vector<unsigned long int> & _p){
	NodeChild::printHierarchy(_startDepth, false, _p);
	childTransform->printHierarchy(_startDepth, _last, _p);
}