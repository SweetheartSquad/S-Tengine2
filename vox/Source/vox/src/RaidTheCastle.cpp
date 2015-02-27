#pragma once

#include "RaidTheCastle.h"
#include "Boulder.h"
#include "Catapult.h"
#include "Box2DSprite.h"
#include "shader/BaseComponentShader.h"

RaidTheCastle::RaidTheCastle(Game* _game):
	PuppetScene(_game)
{
	loadCatapult();
}

RaidTheCastle::~RaidTheCastle(){
}

void RaidTheCastle::update(Step* _step){
	PuppetScene::update(_step);
	if(catapult->ready){
		loadCatapult();
	}
}

void RaidTheCastle::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	PuppetScene::render(_matrixStack, renderOptions);
}

void RaidTheCastle::load(){
	PuppetScene::load();
}

void RaidTheCastle::unload(){
	PuppetScene::unload();
}

void RaidTheCastle::loadCatapult(){
	/*Boulder * boulder = new Boulder(world, PuppetScene::ITEM, PuppetScene::PLAYER | PuppetScene::STRUCTURE | PuppetScene::ITEM);
	boulder->setShader(shader, true);
	addChild(boulder);
	boulder->addToScene(this);
	boulder->translateComponents(glm::vec3(catapult->base->getCorrectedWidth() * 0.8, catapult->base->getCorrectedHeight(), 0));
	catapult->cooldownCnt = 0.f;*/
	catapult->ready = true;
}