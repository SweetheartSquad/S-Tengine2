#include "Game.h"

//these shouldn't be global
double lastTime = glfwGetTime();
int nbFrames = 0;

Game::Game(bool isRunning):
	isRunning(isRunning),
	renderSystem(&RenderSystem::getInstance()),
	children(new std::vector<Entity *>()),
	scenes(new std::map<std::string, Scene *>()),
	printFPS(true),
	keyboard(&Keyboard::getInstance()),
	mouse(&Mouse::getInstance())
{
}

Game::~Game(void){
	RenderSystem::destroyRenderSystem();
}

void Game::update(void){
	vox::calculateDeltaTimeCorrection();
	if(printFPS){
		printFps();
	}
	if(keyboard->keyDown(GLFW_KEY_ESCAPE)){
		glfwSetWindowShouldClose(vox::currentContext, true);
	}
	currentScene->update();
}

void Game::draw(void){
	currentScene->draw();
}

void Game::manageInput(){
	keyboard->update();
	mouse->update();
}

void Game::printFps(){
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0){
		// If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f FPS\n", double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}
