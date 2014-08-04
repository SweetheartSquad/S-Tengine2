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
	Game(bool running);
	~Game(void);

	bool isRunning;
	GLFWwindow *glfwWindow;
	RenderSystem *renderSystem;
	std::string s;
	std::vector<Entity*> *children;
	std::map<std::string, Scene*> *scenes;
	void update(void);
	void draw(void);
};

