#pragma once

#include "LayeredScene.h"

#include "Game.h"
#include "MatrixStack.h"
#include "Entity.h"
#include "Camera.h"
#include <OrthographicCamera.h>

LayeredScene::LayeredScene(Game * _game, unsigned long int _numLayers) :
	Scene(_game),
	numLayers(_numLayers)
{
	layers.resize(_numLayers);
}

void LayeredScene::update(Step * _step){
	Scene::update(_step);
}

void LayeredScene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	glfwMakeContextCurrent(glfwGetCurrentContext());
	float ratio;
	ratio = game->viewPortWidth / static_cast<float>(game->viewPortHeight);

	glEnable(GL_SCISSOR_TEST);

	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

	glViewport(game->viewPortX, game->viewPortY, game->viewPortWidth, game->viewPortHeight);
	glScissor(game->viewPortX, game->viewPortY, game->viewPortWidth, game->viewPortHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	matrixStack->setProjectionMatrix(activeCamera->getProjectionMatrix());
	matrixStack->setViewMatrix(activeCamera->getViewMatrix());

    glDisable(GL_DEPTH_TEST);
	for(std::vector<Entity *> & layer : layers){
		for(Entity * e : layer){
			e->render(matrixStack, renderOptions);
		}
	}
	
	
	OrthographicCamera cam(-1920.f, 0, 0, 1080.f, -1000.f, 1000.f);
	
	matrixStack->setProjectionMatrix(cam.getProjectionMatrix());
	matrixStack->setViewMatrix(cam.getViewMatrix());

	for(Entity * e : uiLayer){
		e->render(matrixStack, renderOptions);
	}
}

void LayeredScene::addChild(Entity* _child){
	std::cout << "LayeredScene::addChild not implemented; call didn't do anything" << std::endl;
}

void LayeredScene::removeChild(Entity* _child){
	Scene::removeChild(_child);
	for(std::vector<Entity *> & layer : layers){
		for(signed long int j = layer.size()-1; j >= 0; --j){
			if(layer.at(j) == _child){
				layer.erase(layer.begin() + j);
			}
		}
	}
}


void LayeredScene::addUIChild(Entity* _child){
	children.push_back(_child);
	uiLayer.push_back(_child);
}
void LayeredScene::removeUIChild(Entity* _child){
	Scene::removeChild(_child);
	for(signed long int j = uiLayer.size()-1; j >= 0; --j){
		if(uiLayer.at(j) == _child){
			uiLayer.erase(uiLayer.begin() + j);
		}
	}
}

void LayeredScene::addChild(Entity* _child, unsigned long int _layer){
	if(_layer < numLayers){
		children.push_back(_child);
		layers.at(_layer).push_back(_child);
	}else{
		std::cout << "Scene does not have a layer " << _layer << std::endl;
	}
}