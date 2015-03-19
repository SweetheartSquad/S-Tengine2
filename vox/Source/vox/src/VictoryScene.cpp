#pragma once

#include <VictoryScene.h>

#include <PuppetController.h>
#include <PuppetCharacter.h>
#include <Box2DSprite.h>
#include <PuppetGame.h>
#include <shader\BaseComponentShader.h>
#include <FollowCamera.h>
#include <PuppetContactListener.h>

VictoryScene::VictoryScene(PuppetGame * _game, std::vector<PuppetCharacter *> _players):
	PuppetScene(_game, 10)
{
	populateBackground();
	cl = new PuppetContactListener(this);
	for(unsigned long int i=0; i<_players.size(); ++i){
		players.push_back(new PuppetCharacter(_players.at(i), world));
		players.at(i)->setShader(shader, true);
		addChild(players.at(i), 1);
		players.at(i)->addToLayeredScene(this, 1);
		players.at(i)->rootComponent->maxVelocity = b2Vec2(10, 10);
		switch(i){
			case 1:
				static_cast<PuppetGame *>(game)->puppetController1->puppetCharacter = players.at(i);
				break;
			case 2:
				static_cast<PuppetGame *>(game)->puppetController2->puppetCharacter = players.at(i);
				break;
			case 3:
				static_cast<PuppetGame *>(game)->puppetController3->puppetCharacter = players.at(i);
				break;
			case 4:
				static_cast<PuppetGame *>(game)->puppetController4->puppetCharacter = players.at(i);
				break;
		}
		players.at(i)->translateComponents(glm::vec3(20.0f * i, 35, 0.f));
		gameCam->addTarget(players.at(i)->torso);
	}
}

VictoryScene::~VictoryScene(){
}

void VictoryScene::update(Step * _step){
	PuppetScene::update(_step);
}

void VictoryScene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	PuppetScene::render(_matrixStack, _renderStack);
}

void VictoryScene::load(){
	PuppetScene::load();
}

void VictoryScene::unload(){
	PuppetScene::unload();
}