#define GLFW_INCLUDE_NONE

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include "Entity.h"
#include "Cube.h"
#include <iostream>
#include "Game.h"
#include <TestGame.h>
#include "Mouse.h"
#include "Vox.h"

TestGame *game;

static void error_callback(int _error, const char* _description){
	fputs(_description, stderr);
}

static void setMousePostionCallback(GLFWwindow *_window, double _x, double _y){
	Mouse *mouse = &Mouse::getInstance();
	mouse->mousePositionListener(_x, _y);
}
int main(void){
	vox::setGlfwWindowHints();

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()){
		exit(EXIT_FAILURE);
	}

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	int w = mode->width;
	int	h = mode->height;

	window = glfwCreateWindow(w/2, h/2, "VOX", nullptr, nullptr);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	vox::initWindow(window);
	glfwMakeContextCurrent(window);
	vox::currentContext = window;

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err){
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "\tERROR: %s\n", glewGetErrorString(err));
	}

	int screenHeight;
	int screenWidth;

	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	glfwSetCursorPos(window, screenWidth/2, screenHeight/2);
	game = new TestGame(true);

	while (game->isRunning){
		glfwPollEvents();

		game->update();
		game->draw();
		glfwSwapBuffers(vox::currentContext);
		game->manageInput();

		game->isRunning = !glfwWindowShouldClose(vox::currentContext);
	}

	glfwDestroyWindow(vox::currentContext);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}