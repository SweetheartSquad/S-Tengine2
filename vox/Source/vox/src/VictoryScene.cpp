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
	PuppetScene(_game, 10, SCENE_WIDTH, 25.f)
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
		players.push_back(_players.at(i)->clone(world, this));

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

	float maxHeight = 9.f;

	// should have a better way, what about ai characters?
	//if(_sortedPuppets.front()->score != _sortedPuppets.back()->score){
	for(unsigned long int i = 0; i < players.size(); ++i){
		b2Vec2 * verts = new b2Vec2[4];

		verts[0].x = (i+0.5f) * 20.f;
		verts[1].x = (i+0.5f) * 20.f;
		verts[2].x = (i+1.5f) * 20.f;
		verts[3].x = (i+1.5f) * 20.f;
		
		float height = maxHeight * players.at(i)->score+1.f;
		verts[0].y = 0;
		verts[1].y = height;
		verts[2].y = height;
		verts[3].y = 0;

		b2PolygonShape shape;
		shape.Set(verts, 4);

		Box2DSprite * podiumStand = new Box2DSprite(world, b2_staticBody, false);
        podiumStand->mesh->vertices.clear();
        float r = (players.at(i)->id == 0 || players.at(i)->id == 3) ? 1.f : 0.f;
        float g = (players.at(i)->id == 0 || players.at(i)->id == 1) ? 1.f : 0.f;
        float b = (players.at(i)->id == 2) ? 1.f : 0.f;
		for(int j = 0; j < 4; ++j){
			podiumStand->mesh->pushVert(Vertex(
				verts[j].x, verts[j].y, 0.f,
				r, g, b, 1.f));
		}
		podiumStand->mesh->polygonalDrawMode = GL_QUADS;
		podiumStand->mesh->dirty = true;
		podiumStand->setShader(shader, true);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 1.f;

		b2Filter sf;
		sf.categoryBits = PuppetGame::kGROUND;
		fd.filter = sf;
		podiumStand->body->CreateFixture(&fd);

		addChild(podiumStand, 0);

		players.at(i)->translateComponents(glm::vec3(0.f, height, 0.f));

		delete verts;
	}

	gameCam->useBounds = true;
	gameCam->minBounds.y = 0;
	gameCam->minBounds.height = sceneHeight;
}

VictoryScene::~VictoryScene(){
}

void VictoryScene::update(Step * _step){
	PuppetScene::update(_step);
}

void VictoryScene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	PuppetScene::render(_matrixStack, _renderOptions);
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
		removeChild(countDownNumbers.at(countDown));
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