#pragma once

#include <LayeredScene.h>

#include <Game.h>
#include <MatrixStack.h>
#include <RenderOptions.h>
#include <Entity.h>
#include <Camera.h>
#include <OrthographicCamera.h>

LayeredScene::LayeredScene(Game * _game, unsigned long int _numLayers) :
	Scene(_game),
	numLayers(_numLayers)
{
	for(unsigned long int i = 0; i < _numLayers; ++i) {
		Transform * trans = new Transform();
		childTransform->addChild(trans);
		layers.push_back(trans);
		depthEnabled.push_back(false);
	}
}

void LayeredScene::update(Step * _step){
	Scene::update(_step);
}

void LayeredScene::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	glfwMakeContextCurrent(glfwGetCurrentContext());
	float ratio;
	ratio = game->viewPortWidth / static_cast<float>(game->viewPortHeight);

	glEnable(GL_SCISSOR_TEST);

	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	_renderOptions->clear();

	glViewport(game->viewPortX, game->viewPortY, game->viewPortWidth, game->viewPortHeight);
	glScissor(game->viewPortX, game->viewPortY, game->viewPortWidth, game->viewPortHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	_matrixStack->pushMatrix();
	_matrixStack->resetCurrentMatrix();
	_matrixStack->setProjectionMatrix(&activeCamera->getProjectionMatrix());
	_matrixStack->setViewMatrix(&activeCamera->getViewMatrix());

	for(unsigned long int i = 0; i < layers.size(); ++i){
		if(depthEnabled.at(i)){
			glEnable(GL_DEPTH_TEST);
		}else{
			glDisable(GL_DEPTH_TEST);
		}
		layers.at(i)->render(_matrixStack, _renderOptions);
	}

	int sceneWidth, sceneHeight;
	glfwGetWindowSize(sweet::currentContext, &sceneWidth, &sceneHeight);
}

void LayeredScene::removeChild(NodeChild * _child){
	for(Transform * layer : layers){
		layer->removeChild(_child);
	}
}

void LayeredScene::addChild(NodeChild * _child, unsigned long int _layer, bool _underNewTransform){
	if(_layer < numLayers){
		layers.at(_layer)->addChild(_child, _underNewTransform);
	}else{
		Log::error("Attempted to add child to a layer that doesn't exist.");
	}
}