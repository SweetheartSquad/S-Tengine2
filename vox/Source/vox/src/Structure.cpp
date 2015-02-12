#pragma once

#include "Structure.h"
#include "Box2DSprite.h"
#include "Box2DWorld.h"
#include "shader/Shader.h"
#include "CharacterComponent.h"
#include "Scene.h"
#include "BitmapFont.h"

#include "Texture.h"

std::vector<StructureComponentTexture *> Structure::catapultTexPacks;
std::vector<StructureComponentTexture *> Structure::leverTexPacks;
std::vector<StructureComponentTexture *> Structure::fortificationTexPacks;

void Structure::init(){
	catapultTexPacks.push_back(new StructureComponentTexture(new Texture("../assets/structure components/catapult/CatapultBase.png", 512, 512, true, true),	500,	300)); 
	catapultTexPacks.push_back(new StructureComponentTexture(new Texture("../assets/structure components/catapult/CatapultFlinger.png", 512, 512, true, true),	500,	100));
}

int16 Structure::gGroupIndex = 0;

StructureComponentTexture::StructureComponentTexture(Texture * _texture, float _width, float _height) :
	texture(_texture),
	width(_width),
	height(_height)
{
}

Structure::Structure(Box2DWorld * _world, int16 _categoryBits, int16 _maskBits) :
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

Structure::~Structure(){
	while(components.size() > 0){
		delete *components.back();
	}
	components.clear();
}

void Structure::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	MeshEntity::render(_matrixStack, _renderStack);
}

void Structure::update(Step * _step){
	MeshEntity::update(_step);
}


void Structure::setShader(Shader * _shader, bool _configureDefaultVertexAttributes){
	MeshEntity::setShader(_shader, _configureDefaultVertexAttributes);
	for(CharacterComponent ** c : components){
		if(*c != nullptr){
			(*c)->setShader(_shader, _configureDefaultVertexAttributes);
		}
	}
}

void Structure::addToScene(Scene * _scene){
	for(CharacterComponent ** c : components){
		if(*c != nullptr){
			_scene->addChild(*c);
		}
	}
}

void Structure::translateComponents(glm::vec3 _translateVector){
	for(CharacterComponent ** c : components){
		if(*c != nullptr){
			(*c)->setTranslationPhysical(_translateVector, true);
		}
	}
}
