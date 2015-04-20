#pragma once

#include <Scene.h>

class LD32_StartupScene : public Scene{
public:
	LD32_StartupScene(Game * _game);

	void update(Step * _step) override;
};