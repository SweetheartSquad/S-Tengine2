#pragma once

#include "Entity.h"
#include "RenderOptions.h"
#include "MatrixStack.h"

Entity::Entity(MeshInterface * _mesh, Transform * _transform, Shader * _shader):
	mesh(_mesh),
	NodeAnimatable(_transform),
	shader(_shader),
	NodeHierarchical(nullptr)
{
	if(mesh != nullptr && shader != nullptr){
		reset();
	}
}

Entity::~Entity(void){
	delete transform;
	delete mesh;
	if(shader != nullptr){
		shader->decrementAndDelete();		
	}
	transform = nullptr;
	mesh = nullptr;
	shader = nullptr;
}

void Entity::draw(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	//push transform
	if(_matrixStack != nullptr && _renderStack != nullptr){
		_matrixStack->pushMatrix();
		_matrixStack->applyMatrix(transform->getModelMatrix());
	
		if(mesh != nullptr){
			mesh->load();
			mesh->clean();
		}
		if(_renderStack->overrideShader == nullptr){
			_renderStack->shader = shader;
		}else{
			_renderStack->shader = _renderStack->overrideShader;
		}
		if(mesh != nullptr){
			mesh->render(_matrixStack, _renderStack);
		}
		for(Node * child : children){
			dynamic_cast<Entity *>(child)->draw(_matrixStack, _renderStack);
		}
		//pop transform
		_matrixStack->popMatrix();
	}
}

void Entity::update(){
}

void Entity::addChild(Entity * _child){
	NodeHierarchical::addChild(_child);
	if(transform != nullptr){
		transform->addChild(_child->transform);	
	}
}

void Entity::removeChildAtIndex(int _index){
	NodeHierarchical::removeChildAtIndex(_index);
	if(transform != nullptr){
		transform->removeChildAtIndex(_index);
	}
}

void Entity::setShader(Shader * _shader, bool _confiugreDefaultAttributes){
	shader = _shader;
	if(_confiugreDefaultAttributes){
		if(mesh != nullptr){
			reset();
		}
	}
}

void Entity::setShaderOnChildren(Shader * _shader){
	for(NodeHierarchical * entity : children){
		(dynamic_cast<Entity*>(entity))->setShaderOnChildren(_shader);
	}
	setShader(_shader, true);
}

void Entity::unload(){
	for(Node * child : children){
		dynamic_cast<Entity *>(child)->unload();
	}
	if(mesh != nullptr){
		mesh->unload();
	}
	if(shader != nullptr){
		shader->unload();	
	}
}

void Entity::reset(){
	for(Node * child : children){
		dynamic_cast<Entity *>(child)->reset();
	}

	if(mesh != nullptr){
		mesh->load();
		mesh->clean();	
	}
	
	if(shader != nullptr){
		shader->load();
		if(mesh != nullptr ){
			mesh->configureDefaultVertexAttributes(shader);
		}
	}
}

void Entity::updateAnimation(Step* step){
	NodeAnimatable::update(step);
	for(int i = 0; i < children.size(); i++){
		dynamic_cast<Entity *>(children.at(i))->updateAnimation(step);
	}
}