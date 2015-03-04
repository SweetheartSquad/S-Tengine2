#pragma once

#include "LayeredScene.h"

#include "Game.h"
#include "MatrixStack.h"
#include "Entity.h"
#include "Camera.h"

LayeredScene::LayeredScene(Game * _game, unsigned long int _numLayers) :
	Scene(_game),
	numLayers(_numLayers)
{
	layers.resize(numLayers);
}

void LayeredScene::update(Step * _step){
	Scene::update(_step);
}

void LayeredScene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	//glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
	glfwMakeContextCurrent(glfwGetCurrentContext());
	float ratio;
	ratio = game->viewPortWidth / static_cast<float>(game->viewPortHeight);

	glEnable(GL_SCISSOR_TEST);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);


	//glBlendFunc(GL_ONE, GL_ZERO);
	glEnable (GL_BLEND);

	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

	glViewport(game->viewPortX, game->viewPortY, game->viewPortWidth, game->viewPortHeight);
	glScissor(game->viewPortX, game->viewPortY, game->viewPortWidth, game->viewPortHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//glEnable(GL_DEPTH_TEST);
     //glAlphaFunc ( GL_GREATER, 0.1 ) ;
     glEnable ( GL_ALPHA_TEST ) ;

	//Back-face culling
	//glEnable (GL_CULL_FACE); // cull face
    //glCullFace (GL_BACK); // cull back face

	//glFrontFace (GL_CW); // GL_CCW for counter clock-wise, GL_CW for clock-wise

	matrixStack->projectionMatrix = camera->getProjectionMatrix();
	matrixStack->viewMatrix		  = camera->getViewMatrix();

    glDisable(GL_DEPTH_TEST);
	for(std::vector<Entity *> layer : layers){
		for(Entity * e : layer){
			e->render(matrixStack, renderOptions);
		}
	}
}

void LayeredScene::addChild(Entity* _child){
	std::cout << "LayeredScene::addChild not implemented; call didn't do anything" << std::endl;
}

void LayeredScene::addChild(Entity* _child, unsigned long int _layer){
	if(_layer < numLayers){
		children.push_back(_child);
		layers.at(_layer).push_back(_child);
	}else{
		std::cout << "Scene does not have a layer " << _layer << std::endl;
	}
}