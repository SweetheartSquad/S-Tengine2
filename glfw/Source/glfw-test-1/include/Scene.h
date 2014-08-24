#pragma once

#include <iostream>
#include "Entity.h"
#include "RenderSystem.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Camera.h"

class Scene
{

public:
	Scene(void);
	virtual ~Scene(void);

	RenderSystem *renderSystem;
	Keyboard* keyboard;
	Mouse* mouse;
	std::vector<Entity*> *children;

	Camera *camera;

	virtual void update(void);
	virtual void draw();

	void addChild(Entity *child);


};

