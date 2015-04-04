#pragma once

#include "Character.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"
#include "CharacterComponent.h"
#include "Scene.h"
#include "LayeredScene.h"
#include "BitmapFont.h"

#include "Texture.h"

int16 Character::gGroupIndex = 0;

ComponentTexture::ComponentTexture(Texture * _texture, float _width, float _height) :
	texture(_texture),
	width(_width),
	height(_height)
{
}

Character::Character(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex, bool _ai) :
	MeshEntity(nullptr, transform), // THERE ARE TWO TRANSFORM NODES HERE WHEN THERE SHOULD BE ONE
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	world(_world),
	componentScale(0.0025f),
	groupIndex(--gGroupIndex),
	categoryBits(_categoryBits),
	maskBits(_maskBits),
	ai(_ai)
{
}

Character::~Character(){
	while(components.size() > 0){
		delete *components.back();
	}
	components.clear();
}

void Character::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	MeshEntity::render(_matrixStack, _renderOptions);
}

void Character::update(Step * _step){
	MeshEntity::update(_step);
}


void Character::setShader(Shader * _shader, bool _configureDefaultVertexAttributes){
	MeshEntity::setShader(_shader, _configureDefaultVertexAttributes);
	for(CharacterComponent ** c : components){
		if(*c != nullptr){
			(*c)->setShader(_shader, _configureDefaultVertexAttributes);
		}
	}
}

void Character::addToScene(Scene * _scene){
	for(CharacterComponent ** c : components){
		if(*c != nullptr){
			_scene->addChild(*c);
		}
	}
}

void Character::addToLayeredScene(LayeredScene * _scene, unsigned long int _layer){
	for(CharacterComponent ** c : components){
		if(*c != nullptr){
			_scene->addChild(*c, _layer);
		}
	}
}

void Character::translateComponents(glm::vec3 _translateVector){
	for(CharacterComponent ** c : components){
		if(*c != nullptr){
			(*c)->setTranslationPhysical(_translateVector, true);
		}
	}
}
