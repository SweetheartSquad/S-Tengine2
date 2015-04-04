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

int16 Box2DSuperSprite::gGroupIndex = 0;

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
		/*try{
			delete components.back();
		}catch(std::exception){
		}*/
		components.pop_back();
	}
	components.clear();
}

void Box2DSuperSprite::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	MeshEntity::render(_matrixStack, _renderOptions);

    for (Box2DSprite ** c : components){
        if (*c != nullptr){
            (*c)->render(_matrixStack, _renderOptions);
        }
    }
}

void Box2DSuperSprite::update(Step * _step){
    MeshEntity::update(_step);
    for (Box2DSprite ** c : components){
        if (*c != nullptr){
            (*c)->update(_step);
        }
    }
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
	/*for(Box2DSprite ** c : components){
		if(*c != nullptr){
			_scene->addChild(*c);
		}
	}*/
}
void Box2DSuperSprite::addToLayeredScene(LayeredScene * _scene, unsigned long int _layer){
	scene = _scene;
	sceneLayer = _layer;
	/*for(Box2DSprite ** c : components){
		if(*c != nullptr){
			_scene->addChild(*c, _layer);
		}
	}*/
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
		(*c)->setUserData(_data);
	}
}

void Box2DSuperSprite::setGroupIndex(int16 _groupIndex){
	for(Box2DSprite ** c : components){
		(*c)->setGroupIndex(_groupIndex);
	}
}

void Box2DSuperSprite::addComponent(Box2DSprite * _component){
	components.push_back(&_component);
	/*if(scene != nullptr){
		if(sceneLayer != -1){
			static_cast<LayeredScene *>(scene)->addChild(_component, sceneLayer);
		}else{
			scene->addChild(_component);
		}
    }*/
}

void Box2DSuperSprite::snapComponents(Box2DSprite * _sprite){
	glm::vec3 currentPos = getPos(false);
	glm::vec3 snapPos = glm::vec3(_sprite->body->GetPosition().x, _sprite->body->GetPosition().y, 0);
	
	translateComponents(glm::vec3(snapPos.x - currentPos.x, snapPos.y - currentPos.y, 0));
}

void Box2DSuperSprite::load(){
	MeshEntity::load();
	for(Box2DSprite ** c : components){
		(*c)->load();
	}
}
void Box2DSuperSprite::unload(){
	MeshEntity::unload();
	for(Box2DSprite ** c : components){
		(*c)->unload();
	}
}