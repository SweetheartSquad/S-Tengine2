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
#include <SoundManager.h>

#define SCENE_WIDTH 100

VictoryScene::VictoryScene(PuppetGame * _game, std::vector<PuppetCharacter *> _players):
	PuppetScene(_game, 10, SCENE_WIDTH, 150.f)
{
	populateBackground();
	cl = new PuppetContactListener(this);

	float maxScore = 1.f;
	int winner = -1;

	for(unsigned long int i = 0; i < _players.size(); ++i){
		if(_players.at(i)->score > maxScore){
			maxScore = _players.at(i)->score;
			winner = _players.at(i)->id;
		}
	}

	if(winner != -1){
		std::stringstream soundName;
		soundName << winner;
		PuppetResourceManager::cheerSounds->play(soundName.str());
		TextureSampler * splashMessageTextureSampler = PuppetResourceManager::winSplashes.at(winner);
		splashMessage = new Sprite(nullptr, new Transform());
		splashMessage->transform->scale(glm::vec3(3, 3, 0));
		splashMessage->mesh->pushTexture2D(splashMessageTextureSampler->texture);
		splashMessage->setShader(shader, true);
		splashMessage->transform->scale(-1, 1, 1);
	}else{
		// tie
	}
	for(unsigned long int i=0; i < _players.size(); ++i){
		players.push_back(new PuppetCharacter(_players.at(i), world));

		players.at(i)->setShader(shader, true);
		addChild(players.at(i), 1);
		players.at(i)->addToLayeredScene(this, 1);
		players.at(i)->rootComponent->maxVelocity = b2Vec2(10, 10);
		players.at(i)->translateComponents(glm::vec3(20.0f * (i+1), 35, 0.f));
		gameCam->addTarget(players.at(i)->torso);
		static_cast<PuppetGame *>(game)->puppetControllers.at(i)->setPuppetCharacter(players.at(i), players.at(i)->id);

		players.at(i)->score /= maxScore;
	}

	/*std::sort(sortedPlayers.begin(), sortedPlayers.end(), PuppetCharacter::compareByScore);
	podium(sortedPlayers);*/

	float maxHeight = 5.f;

	// should have a better way, what about ai characters?
	//if(_sortedPuppets.front()->score != _sortedPuppets.back()->score){
		for(unsigned long int i = 0; i < players.size(); ++i){
			//float height = maxHeight * (players.at(i)->score + 0.25) / 4;
			//float width = SCENE_WIDTH / (players.size()*4);
			Box2DSprite * podiumStand = new Box2DSprite(world, PuppetResourceManager::hand1, b2_staticBody, false, nullptr, new Transform());
			//podiumStand->transform->scale(width, height, 1);
			podiumStand->mesh->uvEdgeMode = GL_REPEAT;
			podiumStand->setShader(shader, true);
			podiumStand->setTranslationPhysical(players.at(i)->rootComponent->body->GetPosition().x, podiumStand->getCorrectedHeight(), 0.f);
			
			
			b2Filter sf;
			sf.categoryBits = PuppetGame::kGROUND;
			podiumStand->createFixture(sf);
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

void VictoryScene::doCountDown(){
	// Remove previous number
	if (countDown <= countDownNumbers.size() - 1){
		// Remove previous number from scene
		// Just copying destroyItem stuff for now
		for(signed long int j = children.size()-1; j >= 0; --j){
			if(children.at(j) == countDownNumbers.at(countDown)){
				children.erase(children.begin() + j);
			}
		}
		for(std::vector<Entity *> * layer : layers){
			for(signed long int j = layer->size()-1; j >= 0; --j){
				if(layer->at(j) == countDownNumbers.at(countDown)){
					layer->erase(layer->begin() + j);
				}
			}
		}
	}
	
	// Decrease countdown
	-- countDown;
	if(countDown == 0){
		// Display countdown
		std::cout << "=========================" << std::endl;
		std::cout << countDown << std::endl;
		std::cout << "idx: " << countDown << std::endl;
		std::cout << "=========================" << std::endl;

		countdownSoundManager->play(std::to_string(countDown));

		// Add new number to scene
		addChild(countDownNumbers.at(countDown), 2);
	}
}