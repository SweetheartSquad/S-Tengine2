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
#include <TextureSampler.h>

#define SCENE_WIDTH 100

VictoryScene::VictoryScene(PuppetGame * _game, std::vector<PuppetCharacter *> _players):
	PuppetScene(_game, 10, SCENE_WIDTH, 150.f)
{
	populateBackground();
	cl = new PuppetContactListener(this);

	std::vector<PuppetCharacter *> sortedPlayers;

	float maxScore = 1.f;

	for(unsigned long int i=0; i<_players.size(); ++i){
		maxScore = std::max(maxScore, _players.at(i)->score);
	}

	for(unsigned long int i=0; i<_players.size(); ++i){
		players.push_back(new PuppetCharacter(_players.at(i), world));
		sortedPlayers.push_back(players.at(i));

		players.at(i)->setShader(shader, true);
		addChild(players.at(i), 1);
		players.at(i)->addToLayeredScene(this, 1);
		players.at(i)->rootComponent->maxVelocity = b2Vec2(10, 10);
		players.at(i)->translateComponents(glm::vec3(20.0f * (i+1), 35, 0.f));
		gameCam->addTarget(players.at(i)->torso);
		static_cast<PuppetGame *>(game)->puppetControllers.at(i)->setPuppetCharacter(players.at(i));

		players.at(i)->score /= maxScore;
	}

	/*std::sort(sortedPlayers.begin(), sortedPlayers.end(), PuppetCharacter::compareByScore);
	podium(sortedPlayers);*/

	float maxHeight = 5.f;

	// should have a better way, what about ai characters?
	//if(_sortedPuppets.front()->score != _sortedPuppets.back()->score){
		for(unsigned long int i = 0; i < players.size(); ++i){
			float height = maxHeight * (players.at(i)->score + 0.25);
			float width = SCENE_WIDTH / (players.size()*2);
			Box2DSprite * podiumStand = new Box2DSprite(world, b2_staticBody, false, nullptr, new Transform(), PuppetResourceManager::ground1, 512/2.f, 512/2.f, 0, 0);
			podiumStand->transform->scale(width, height, 1);
			podiumStand->mesh->uvEdgeMode = GL_REPEAT;
			podiumStand->setShader(shader, true);
			podiumStand->setTranslationPhysical(players.at(i)->rootComponent->body->GetPosition().x, podiumStand->getCorrectedHeight(), 0.f);
			podiumStand->createFixture(b2Filter());
			addChild(podiumStand, 1);

			players.at(i)->translateComponents(glm::vec3(0.f, podiumStand->getCorrectedHeight(), 0.f));
		}
	/*}else{
		// NO WINNER!
	}*/
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