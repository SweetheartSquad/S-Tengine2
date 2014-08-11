#pragma once 

#include <iostream>
#include "Game.h"

class TestGame:public Game
{
public:

	explicit TestGame(bool running);
	~TestGame();
	void update() override;
	void draw() override;
};