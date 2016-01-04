#pragma once

#include <LayeredScene.h>

#include <MatrixStack.h>
#include <RenderOptions.h>
#include <Entity.h>
#include <Camera.h>
#include <Light.h>

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
	// don't bother doing any work if we aren't rendering anyway
	if(!isVisible()){
		return;
	}

	// render options
	glEnable(GL_SCISSOR_TEST);
	if(_renderOptions->depthEnabled){
		glEnable(GL_DEPTH_TEST);
	}else{
		glDisable(GL_DEPTH_TEST);
	}if(_renderOptions->alphaEnabled){
		glEnable(GL_ALPHA_TEST);
		//glAlphaFunc ( GL_GREATER, 0.1 ) ;
		//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
	}else{
		glDisable(GL_ALPHA_TEST);
	}
	_renderOptions->lights = &lights;
	//Back-face culling
	//glEnable (GL_CULL_FACE); // cull face
	//glCullFace (GL_BACK); // cull back face
	//glFrontFace (GL_CW); // GL_CCW for counter clock-wise, GL_CW for clock-wise


	// matrix stack
	_matrixStack->pushMatrix();
	_matrixStack->resetCurrentMatrix();
	_matrixStack->setCamera(activeCamera);

	// render
	for(unsigned long int i = 0; i < layers.size(); ++i){
		if(depthEnabled.at(i)){
			glEnable(GL_DEPTH_TEST);
		}else{
			glDisable(GL_DEPTH_TEST);
		}
		layers.at(i)->render(_matrixStack, _renderOptions);
	}

	_matrixStack->popMatrix();

	checkForGlError(false);

	// clean the lights
	for(Light * l : lights){
		l->lightDirty = false;
	}
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