#pragma once

#include "PuppetScene.h"

PuppetScene::PuppetScene(Game * _game):
	Scene(_game)
{
}

PuppetScene::~PuppetScene(){
}

void PuppetScene::update(Step * _step){
	Scene::update(_step);
}

void PuppetScene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	Scene::render(_matrixStack, _renderStack);
}

void PuppetScene::load(){
	Scene::load();
}

void PuppetScene::unload(){
	Scene::unload();
}