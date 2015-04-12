#pragma once

#include <StartupScene.h>
#include <PuppetGame.h>
#include <PuppetResourceManager.h>
#include <Sprite.h>
#include <MeshInterface.h>
#include <shader\BaseComponentShader.h>
#include <SoundManager.h>
#include <PuppetController.h>

StartupScene::StartupScene(PuppetGame * _game) :
	PuppetScene(_game, 20.f)
{
	splashMessage = new Sprite(nullptr, new Transform());
	splashMessage->mesh->pushTexture2D(PuppetResourceManager::startupSplash);
	splashMessage->setShader(shader, true);
	splashMessage->transform->translate(1920.f*0.5, 1080.f*0.5f, 0);

	populateBackground();
}
void StartupScene::complete(std::string _switchTo){
	if (_switchTo == ""){
    PuppetGame * pg = static_cast<PuppetGame *>(game);
	pg->loadRandomScene();
	}else{
		PuppetScene::complete(_switchTo);
	}
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

void StartupScene::populateBackground(){
}