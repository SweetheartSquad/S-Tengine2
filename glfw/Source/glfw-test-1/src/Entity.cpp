#pragma once

#include "Entity.h"
#include "RenderOptions.h"
#include "MatrixStack.h"
#include "NodeResource.h"

Entity::Entity(Transform * _transform) :
	NodeTransformable(_transform),
	NodeAnimatable(),
	NodeHierarchical(),
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
	for(int i = 0; i < children.size(); i++){
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
		Entity * e = dynamic_cast<Entity *>(child);
		if(e != nullptr){
			e->unload();	
		}else
		{
			NodeResource * nr = dynamic_cast<NodeResource *>(child);
			if(nr != nullptr){
				nr->unload();	
			}	
		}	
	}
}

void Entity::load(){
	for(NodeChild * child : children){
		Entity * e = dynamic_cast<Entity *>(child);
		if(e != nullptr){
			e->load();	
		}else
		{
			NodeResource * nr = dynamic_cast<NodeResource *>(child);
			if(nr != nullptr){
				nr->load();	
			}	
		}	
	}
}