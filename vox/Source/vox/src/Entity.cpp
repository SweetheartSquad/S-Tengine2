#pragma once

#include "Entity.h"
#include "RenderOptions.h"
#include "MatrixStack.h"
#include "node/NodeResource.h"
#include "Sprite.h"

#include "glew\glew.h"

Entity::Entity(Transform * _transform) :
	NodeTransformable(_transform),
	NodeAnimatable(),
	NodeHierarchical(),
	NodeLoadable(),
	NodeChild(nullptr)
{
}

Entity::~Entity(void){
	delete transform;	
	transform = nullptr;
}

void Entity::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	for(unsigned long int i = 0; i < children.size(); i++){
		NodeRenderable * nr = dynamic_cast<NodeRenderable *>(children.at(i));
		if(nr != nullptr){
			nr->render(_matrixStack, _renderStack);	
		}
	}
}

void Entity::update(Step * _step){
	NodeAnimatable::update(_step);
	for(unsigned long int i = 0; i < children.size(); ++i){
		NodeUpdatable * nu = dynamic_cast<NodeUpdatable *>(children.at(i));
		if(nu != nullptr){
			nu->update(_step);	
		}
	}
}

void Entity::removeChildAtIndex(int _index){
	NodeHierarchical::removeChildAtIndex(_index);
}

void Entity::unload(){
	for(NodeChild * child : children){
		NodeLoadable * nl = dynamic_cast<NodeLoadable *>(child);
		if(nl != nullptr){
			nl->unload();	
		}
	}
	
	NodeLoadable::unload();
}

void Entity::load(){
	for(NodeChild * child : children){
		NodeLoadable * nl = dynamic_cast<NodeLoadable *>(child);
		if(nl != nullptr){
			nl->load();	
		}
	}
	
	NodeLoadable::load();
}