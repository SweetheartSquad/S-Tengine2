#pragma once

#include <StartupScene.h>
#include <PuppetGame.h>
#include <PuppetResourceManager.h>
#include <Sprite.h>
#include <MeshInterface.h>
#include <shader\BaseComponentShader.h>
#include <SoundManager.h>

StartupScene::StartupScene(PuppetGame * _game) :
	PuppetScene(_game, 20.f)
{
	splashMessage = new Sprite(nullptr, new Transform());
	splashMessage->transform->scale(glm::vec3(3, 3, 0));
	splashMessage->mesh->pushTexture2D(PuppetResourceManager::startupSplash);
	splashMessage->setShader(shader, true);
	splashMessage->transform->scale(-1, 1, 1);
}
void StartupScene::complete(std::string _switchTo){
    PuppetGame * pg = static_cast<PuppetGame *>(game);
	pg->loadRandomScene();
}

void StartupScene::update(Step * _step){
	PuppetScene::update(_step);
	if(!splashSoundPlayed){
		PuppetResourceManager::splashSounds->play("Startup-alt");
		splashSoundPlayed = true;
	}
}
void StartupScene::triggerVictoryState(){
	// intentionally left blank: since there are no players, this function will be called automatically at the start of the scene
}