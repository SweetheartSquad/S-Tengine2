#pragma once

#include <VictoryScene.h>

#include <PuppetController.h>
#include <PuppetCharacter.h>
#include <Box2DSprite.h>
#include <PuppetGame.h>
#include <shader\BaseComponentShader.h>
#include <FollowCamera.h>
#include <PuppetContactListener.h>
#include <MeshInterface.h>

VictoryScene::VictoryScene(PuppetGame * _game, std::vector<PuppetCharacter *> _players):
	PuppetScene(_game, 10)
{
	populateBackground();
	cl = new PuppetContactListener(this);

	std::vector<PuppetCharacter *> sortedPlayers;

	for(unsigned long int i=0; i<_players.size(); ++i){
		players.push_back(new PuppetCharacter(_players.at(i), world));
		sortedPlayers.push_back(players.at(i));

		players.at(i)->setShader(shader, true);
		addChild(players.at(i), 1);
		players.at(i)->addToLayeredScene(this, 1);
		players.at(i)->rootComponent->maxVelocity = b2Vec2(10, 10);
		players.at(i)->translateComponents(glm::vec3(20.0f * i, 35, 0.f));
		gameCam->addTarget(players.at(i)->torso);
		static_cast<PuppetGame *>(game)->puppetControllers.at(i)->setPuppetCharacter(players.at(i));
	}

	std::sort(sortedPlayers.begin(), sortedPlayers.end(), PuppetCharacter::compareByScore);
	podium(sortedPlayers);
}

VictoryScene::~VictoryScene(){
}

void VictoryScene::podium(std::vector<PuppetCharacter *> _sortedPuppets){
	float maxHeight = 20.f;

	// should have a better way, what about ai characters?
	if(_sortedPuppets.front()->score != _sortedPuppets.back()->score){
		for(unsigned long int i = 0; i < _sortedPuppets.size(); ++i){
			float height = maxHeight / (i + 1);

			Box2DSprite podiumStand = Box2DSprite(world, b2_staticBody, false, nullptr, new Transform(), 20.f, height, nullptr);
			podiumStand.mesh->pushTexture2D(PuppetResourceManager::sky);
			podiumStand.mesh->uvEdgeMode = GL_REPEAT;
			podiumStand.setShader(shader, true);
			podiumStand.setTranslationPhysical(0.f, podiumStand.getCorrectedHeight(), 0.f);
			addChild(&podiumStand, 1);

			_sortedPuppets.at(i)->translateComponents(glm::vec3(0.f, podiumStand.getCorrectedHeight() * 2.f, 0.f));
		}
	}else{
		// NO WINNER!
	}
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