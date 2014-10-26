#pragma once

#include<iostream>
#include"Scene.h"
#include"Light.h"
#include"Cube.h"
#include "Vox.h"

class MainScene : public Scene{
public:
	explicit MainScene(Game * _game);
	~MainScene();
	void update() override;
	void render() override;
	void onContextChange() override;
};