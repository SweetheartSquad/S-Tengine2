#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderSystem.h"

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

	void update(void);
	void draw(void);
};

