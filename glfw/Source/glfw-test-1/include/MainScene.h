#pragma once

#include<iostream>
#include"Scene.h"
#include"Cube.h"

class MainScene:public Scene
{
public:
	MainScene();
	~MainScene() override;
	void update() override;
	void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) override;
};