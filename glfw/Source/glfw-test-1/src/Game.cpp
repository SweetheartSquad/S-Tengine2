#include "Game.h"
#include "Keyboard.h"
#include "Mouse.h"

//these shouldn't be global
double lastTime = glfwGetTime();
int nbFrames = 0;

Game::Game(bool _isRunning):
	isRunning(_isRunning),
	children(new std::vector<Entity *>()),
	printFPS(true),
	keyboard(&Keyboard::getInstance()),
	mouse(&Mouse::getInstance())
{
	//Initialize Freetype
	FT_Library  library;

	int error = FT_Init_FreeType( &library );
	if ( std::codecvt_base::error ){
		std::cout<<"Error initializing freetype";
	}
}

Game::~Game(void){
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
	currentScene->render();
}

void Game::manageInput(){
	keyboard->update();
	mouse->update();
}

FT_Library* Game::getFreetypeLibraryReference(){
	return new FT_Library(freetypeLibrary);
}

void Game::printFps(){
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0){
		// If last printf() was more than 1 sec ago
		// printf and reset timer
		printf("%f FPS\n", double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}