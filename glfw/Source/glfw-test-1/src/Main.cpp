
#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include "Entity.h"
#include "Cube.h"
#include <iostream>
#include <gmtl/gmtl.h>
#include <gmtl/Vec.h>
#include "Game.h"
#include <TestGame.h>
#include "Mouse.h"

TestGame *game;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Keyboard *keyboard = &Keyboard::getInstance();

	if(action == GLFW_PRESS)
	{
		keyboard->keyDownListener(key);	
	}else if(action == GLFW_RELEASE)
	{
		keyboard->keyUpListener(key);	
	}
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Mouse *mouse = &Mouse::getInstance();

	if(action == GLFW_PRESS)
	{
		mouse->mouseDownListener(button);
	}else if(action == GLFW_RELEASE)
	{
		mouse->mouseUpListener(button);	
	}
}

static void mousePostionCallback(GLFWwindow *window, double x, double y)
{
	Mouse *mouse = &Mouse::getInstance();
	mouse->mousePositionListener(x, y);
}

static void setMousePostionCallback(GLFWwindow *window, double x, double y)
{
	Mouse *mouse = &Mouse::getInstance();
	mouse->mousePositionListener(x, y);
}

int main(void)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(640, 480, "Simple example", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mousePostionCallback);

	glewExperimental = GL_TRUE; 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "\tERROR: %s\n", glewGetErrorString(err));
	}
	
	int *screenHeight = new int();
	int *screenWidth = new int();

	glfwGetWindowSize(glfwGetCurrentContext(), screenWidth, screenHeight);
	glfwSetCursorPos(window, static_cast<int>(*screenWidth)/2, static_cast<int>(*screenHeight)/2);

	game = new TestGame(true);

	while (game->isRunning)
	{
		glfwPollEvents();
		game->update();
		game->draw();
		game->manageInput();
		game->isRunning = !glfwWindowShouldClose(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}