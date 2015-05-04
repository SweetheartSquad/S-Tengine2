#pragma once

#include "Entity.h"
#include "RenderOptions.h"
#include "MatrixStack.h"
#include "node/NodeResource.h"
#include "Sprite.h"

#include "glew\glew.h"

Entity::Entity() :
	childButNotReally(new Transform())
{
}

Entity::~Entity(void){
	delete childButNotReally;
	childButNotReally = nullptr;
}

void Entity::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	childButNotReally->render(_matrixStack, _renderOptions);
}

void Entity::update(Step * _step){
	childButNotReally->update(_step);
}

void Entity::unload(){
	childButNotReally->unload();
	
	NodeLoadable::unload();
}

void Entity::load(){
	childButNotReally->load();
	
	NodeLoadable::load();
}