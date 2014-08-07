#include "Game.h"
#include <MainScene.h>

double lastTime = glfwGetTime();
int nbFrames = 0;

Game::Game(bool isRunning)
{
	this->isRunning = isRunning;
	this->glfwWindow = glfwGetCurrentContext();
	this->renderSystem = &RenderSystem::getInstance();
	this->children = new std::vector<Entity*>();
	this->scenes = new std::map<std::string, Scene*>();
	this->printFPS = true;
}

Game::~Game(void)
{
	RenderSystem::destroyRenderSystem();
}

void Game::update(void)
{
	if (printFPS)
	{
		printFps();
	}
}

void Game::draw(void)
{
	currentScene->draw();
}

void Game::printFps()
{
	currentScene->update();
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0)
	{
		// If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f FPS\n", double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}