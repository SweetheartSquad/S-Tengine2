#pragma once

#include <iostream>
#include "Entity.h"
#include "RenderSystem.h"
#include "Keyboard.h"
#include "Mouse.h"

class Scene:public Entity
{

public:
	Scene(void);
	~Scene(void);

	RenderSystem *renderSystem;
	Keyboard* keyboard;
	Mouse* mouse;

	void update(void) override;
	void draw(void) override;

};

