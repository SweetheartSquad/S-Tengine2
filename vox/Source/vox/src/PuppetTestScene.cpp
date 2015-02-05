#pragma once

#include "PuppetTestScene.h"

PuppetTestScene::PuppetTestScene(Game* _game):
	PuppetScene(_game)
{

}

PuppetTestScene::~PuppetTestScene(){
}

void PuppetTestScene::update(Step* _step){
	PuppetScene::update(_step);
}

void PuppetTestScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	PuppetScene::render(_matrixStack, renderOptions);
}

void PuppetTestScene::load(){
	PuppetScene::load();
}

void PuppetTestScene::unload(){
	PuppetScene::unload();
}