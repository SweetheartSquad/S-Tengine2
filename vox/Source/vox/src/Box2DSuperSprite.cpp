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

int16 Box2DSuperSprite::gGroupIndex = 0;


TextureSampler::TextureSampler(Texture * _texture, float _width, float _height) :
	NodeResource(true),
	texture(_texture),
	width(_width),
	height(_height)
{
}

void TextureSampler::load(){
	texture->load();
	NodeResource::load();
}

void TextureSampler::unload(){
	texture->unload();
	NodeResource::unload();
}

Box2DSuperSprite::Box2DSuperSprite(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	MeshEntity(nullptr, transform), // THERE ARE TWO TRANSFORM NODES HERE WHEN THERE SHOULD BE ONE
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	world(_world),
	componentScale(0.0025f),
	groupIndex(_groupIndex),
	categoryBits(_categoryBits),
	maskBits(_maskBits),
	rootComponent(nullptr),
	scene(nullptr),
	sceneLayer(-1)
{
}

Box2DSuperSprite::~Box2DSuperSprite(){
	while(components.size() > 0){
		delete *components.back();
		components.pop_back();
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
	scene = _scene;
	sceneLayer = -1;
	for(Box2DSprite ** c : components){
		if(*c != nullptr){
			_scene->addChild(*c);
		}
	}
}
void Box2DSuperSprite::addToLayeredScene(LayeredScene * _scene, unsigned long int _layer){
	scene = _scene;
	sceneLayer = _layer;
	for(Box2DSprite ** c : components){
		if(*c != nullptr){
			_scene->addChild(*c, _layer);
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

void Box2DSuperSprite::setUserData(void * _data){
	for(Box2DSprite ** c : components){
		b2Fixture * f = (*c)->body->GetFixtureList();
		while(f != nullptr){
			f->SetUserData(this);
			f = f->GetNext();
		}
	}
}

void Box2DSuperSprite::setGroupIndex(int16 _groupIndex){
	for(Box2DSprite ** c : components){
		(*c)->setGroupIndex(_groupIndex);
	}
}

void Box2DSuperSprite::addComponent(Box2DSprite * _component){
	components.push_back(&_component);
	if(scene != nullptr){
		if(sceneLayer != -1){
			static_cast<LayeredScene *>(scene)->addChild(_component, sceneLayer);
		}else{
			scene->addChild(_component);
		}
	}
}