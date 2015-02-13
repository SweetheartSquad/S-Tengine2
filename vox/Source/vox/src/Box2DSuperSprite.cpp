#pragma once

#include "Box2DSuperSprite.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"
#include "Scene.h"
#include "BitmapFont.h"

#include "Texture.h"

int16 Box2DSuperSprite::gGroupIndex = 0;

TextureSampler::TextureSampler(Texture * _texture, float _width, float _height) :
	texture(_texture),
	width(_width),
	height(_height)
{
}

Box2DSuperSprite::Box2DSuperSprite(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits) :
	MeshEntity(nullptr, transform), // THERE ARE TWO TRANSFORM NODES HERE WHEN THERE SHOULD BE ONE
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	world(_world),
	componentScale(0.0025f),
	groupIndex(--gGroupIndex),
	categoryBits(_categoryBits),
	maskBits(_maskBits)
{
}

Box2DSuperSprite::~Box2DSuperSprite(){
	while(components.size() > 0){
		delete *components.back();
	}
	components.clear();
}

void Box2DSuperSprite::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	MeshEntity::render(_matrixStack, _renderStack);
}

void Box2DSuperSprite::update(Step * _step){
	MeshEntity::update(_step);
}


void Box2DSuperSprite::setShader(Shader * _shader, bool _configureDefaultVertexAttributes){
	MeshEntity::setShader(_shader, _configureDefaultVertexAttributes);
	for(Box2DSprite ** c : components){
		if(*c != nullptr){
			(*c)->setShader(_shader, _configureDefaultVertexAttributes);
		}
	}
}

void Box2DSuperSprite::addToScene(Scene * _scene){
	for(Box2DSprite ** c : components){
		if(*c != nullptr){
			_scene->addChild(*c, true);
		}
	}
}

void Box2DSuperSprite::translateComponents(glm::vec3 _translateVector){
	for(Box2DSprite ** c : components){
		if(*c != nullptr){
			(*c)->setTranslationPhysical(_translateVector, true);
		}
	}
}
