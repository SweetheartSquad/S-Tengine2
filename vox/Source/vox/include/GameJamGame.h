#pragma once

#include <iostream>
#include "Game.h"

class GameJamGame:public Game{
public:

	explicit GameJamGame(bool _running);
	~GameJamGame();
	void update() override;
	void draw() override;
};