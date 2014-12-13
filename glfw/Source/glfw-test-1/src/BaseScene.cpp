#pragma once

#include "BaseScene.h"
#include "Cube.h"
#include "BaseComponentShader.h"
#include "TextureShaderComponent.h"
#include "MeshInterface.h"
#include "Texture.h";

BaseScene::BaseScene(Game* _game)
	:Scene(_game),
	cube(new Cube(glm::vec3(0, 0, 0), 1)),
	shader(new BaseComponentShader())
{
	shader->components.push_back(new TextureShaderComponent);
	shader->compileShader();

	cube->setShader(shader, true);
	addChild(cube);
}

BaseScene::~BaseScene(){
	delete cube;
	delete shader;
}

void BaseScene::update(){
	Scene::update();

}

void BaseScene::render(){
	Scene::render();
}