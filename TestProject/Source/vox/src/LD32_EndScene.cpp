#pragma once

#include <LD32_EndScene.h>
#include <LD32_Game.h>
#include <LD32_Scene.h>
#include <LD32_ResourceManager.h>
#include <Keyboard.h>
#include <GLFW\glfw3.h>
#include <Sprite.h>
#include <MeshInterface.h>
#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentTexture.h>
#include <OrthographicCamera.h>

LD32_EndScene::LD32_EndScene(Game * _game, bool _won) :
	Scene(_game)
{
	
	BaseComponentShader * shader = new BaseComponentShader(true);
	shader->components.push_back(new ShaderComponentTexture(shader));

	shader->compileShader();
	Sprite * splash = new Sprite();
	splash->mesh->pushTexture2D(_won ? LD32_ResourceManager::endSplash : LD32_ResourceManager::endSplashBad);
	addChild(splash);
	splash->setShader(shader, true);
	splash->transform->translate(-1024,512,-5);
	splash->transform->scale(512,512,1);
	
	OrthographicCamera * cam = new OrthographicCamera(0, 1920, 0, 1080, -10, 10);
	cameras.push_back(cam);
	activeCamera = cam;
	clearColor[0] = 1;
	clearColor[1] = 1;
	clearColor[2] = 1;

}

void LD32_EndScene::update(Step * _step){
	Scene::update(_step);

	if(keyboard->keyJustUp(GLFW_KEY_ENTER)){
		game->scenes.insert(std::pair<std::string, Scene *>("game", new LD32_Scene(game)));
		game->switchScene("game", true);
	}
}