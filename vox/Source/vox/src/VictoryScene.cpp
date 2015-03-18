#pragma once

#include <VictoryScene.h>

#include <PuppetController.h>


VictoryScene::VictoryScene(PuppetGame* _game, PuppetCharacter* _p1, PuppetCharacter* _p2, PuppetCharacter* _p3, PuppetCharacter* _p4):
	PuppetScene(_game, 60),
	playerCharacter1(_p1),
	playerCharacter2(_p2),
	playerCharacter3(_p3),
	playerCharacter4(_p4)
{
}

VictoryScene::~VictoryScene(){
}

void VictoryScene::update(Step* _step){
}

void VictoryScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
}

void VictoryScene::load(){
}

void VictoryScene::unload(){
}