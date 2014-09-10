#pragma once

#include <iostream>
#include "Entity.h"
#include "RenderSystem.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Camera.h"

class Scene{
public:
	Scene(void);
	virtual ~Scene(void);

	std::vector<Entity *> * children;
	Camera * camera;

	//singletons
	Keyboard * keyboard;
	Mouse * mouse;

	virtual void update(void);
	virtual void draw();

	void addChild(Entity * child);
};
