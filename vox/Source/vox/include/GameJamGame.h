#pragma once

#include <iostream>
#include "Game.h"

class GameJamGame:public Game{
public:

	explicit GameJamGame(bool _running);
	~GameJamGame();
	void update(Step * _step) override;
	void draw() override;
};