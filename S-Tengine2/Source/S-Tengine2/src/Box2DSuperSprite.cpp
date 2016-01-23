#pragma once

#include "Box2DSuperSprite.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"
#include "Scene.h"
#include "LayeredScene.h"
#include "BitmapFont.h"
#include <node/NodeResource.h>
#include "Texture.h"
#include <TextureSampler.h>

Box2DSuperSprite::Box2DSuperSprite(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	world(_world),
	componentScale(1.f),
	groupIndex(_groupIndex),
	categoryBits(_categoryBits),
	maskBits(_maskBits),
	rootComponent(nullptr),
	sceneLayer(-1)
{
}

Box2DSuperSprite::~Box2DSuperSprite(){
	components.clear();
}

void Box2DSuperSprite::setShader(Shader * _shader, bool _configureDefaultVertexAttributes){
	shader = _shader;
	for(Box2DSprite ** c : components){
		if(*c != nullptr){
			(*c)->setShader(shader, _configureDefaultVertexAttributes);
		}
	}
	++shader->referenceCount;
}

void Box2DSuperSprite::translateComponents(float _x, float _y, float _z){
	translateComponents(glm::vec3(_x, _y, _z));
}
void Box2DSuperSprite::translateComponents(glm::vec3 _translateVector){
	for(Box2DSprite ** c : components){
		if(*c != nullptr){
			(*c)->translatePhysical(_translateVector, true);
		}
	}
}

void Box2DSuperSprite::setUserData(void * _data){
	for(Box2DSprite ** c : components){
		(*c)->setUserData(_data);
	}
}

void Box2DSuperSprite::setGroupIndex(int16 _groupIndex){
	for(Box2DSprite ** c : components){
		(*c)->setGroupIndex(_groupIndex);
	}
	groupIndex = _groupIndex;
}

void Box2DSuperSprite::addComponent(Box2DSprite ** _component){
	components.push_back(_component);
	childTransform->addChild(*_component);
	if((*_component)->shader == nullptr && shader != nullptr) {
		(*_component)->setShader(shader);
	}
}

void Box2DSuperSprite::snapComponents(Box2DSprite * _sprite){
	glm::vec3 currentPos = getWorldPos();
	glm::vec3 snapPos = glm::vec3(_sprite->body->GetPosition().x, _sprite->body->GetPosition().y, 0);
	
	translateComponents(glm::vec3(snapPos.x - currentPos.x, snapPos.y - currentPos.y, 0));
}