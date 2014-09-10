#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderSystem.h"
#include "Entity.h"
#include "Scene.h"
#include <map>
#include "Keyboard.h"
#include "Mouse.h"
#include "Vox.h"

class Game
{
private:
	void printFps();

public:

	explicit Game(bool _running);
	virtual ~Game(void);

	bool isRunning;
	bool printFPS;

	GLFWwindow *glfwWindow;
	RenderSystem *renderSystem;

	std::vector<Entity*> *children;

	std::map<std::string, Scene*> *scenes;
	Scene *currentScene;

	Keyboard *keyboard;
	Mouse *mouse;

	virtual void update(void);
	virtual void draw(void);
	void manageInput(void);
};
