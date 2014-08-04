#pragma once
#include <iostream>
#include "Entity.h"
#include "RenderSystem.h"

class Scene
{
public:
	Scene(void);
	~Scene(void);

	std::vector<Entity*> *children;
	RenderSystem *renderSystem;

	void update(void);
	void draw(void);
};

