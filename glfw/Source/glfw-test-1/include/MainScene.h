#pragma once

#include<iostream>
#include"Scene.h"
#include"Light.h"
#include"Cube.h"
#include "Vox.h"

class MainScene : public Scene{
public:
	MainScene();
	~MainScene();
	void update() override;
	void draw() override;
};