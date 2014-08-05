#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderSystem.h"
#include "Entity.h"
#include "Scene.h"
#include <map>

class Game
{
private:
	void printFps();

public:
	
	explicit Game(bool running);
	virtual ~Game(void);

	bool isRunning;
	bool printFPS;
	GLFWwindow *glfwWindow;
	RenderSystem *renderSystem;
	std::string s;
	std::vector<Entity*> *children;
	std::map<std::string, Scene*> *scenes;
	Scene *currentScene;
	
	virtual void update(void);
	virtual void draw(void);
};

