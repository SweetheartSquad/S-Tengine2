#pragma once

#include <iostream>
#include "Entity.h"
#include "RenderSystem.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Camera.h"

class Scene:public Entity
{

public:
	Scene(void);
	~Scene(void);

	RenderSystem *renderSystem;
	Keyboard* keyboard;
	Mouse* mouse;

	Camera *camera;

	void update(void) override;
	void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) override;

};

