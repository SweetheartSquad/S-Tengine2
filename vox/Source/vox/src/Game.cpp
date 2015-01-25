#pragma once

#include "Game.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Vox.h"
#include "MatrixStack.h"
#include "VoxRenderOptions.h"

#include <stdlib.h>

//these shouldn't be global
double lastTime = glfwGetTime();
int nbFrames = 0;

Game::Game(bool _isRunning):
	mouse(&Mouse::getInstance()),
	keyboard(&Keyboard::getInstance()),
	isRunning(_isRunning),
	printFPS(true),
	kc_lastKey(0),
	kc_code(0),
	kc_active(false),
	switchingScene(false)
{
}

Game::~Game(void){
	for(auto scene : scenes){
		delete scene.second;
	}
}

void Game::performGameLoop(){
	glfwPollEvents();

	update();
	draw();

	glfwSwapBuffers(vox::currentContext);
	manageInput();
	isRunning = !glfwWindowShouldClose(vox::currentContext);

	if(switchingScene){
		currentScene = scenes.at(newScene);
		switchingScene = false;
		newScene = "";
	}
}

void Game::update(void){
	vox::calculateDeltaTimeCorrection();
	if(printFPS){
		printFps();
	}
	if(keyboard->keyDown(GLFW_KEY_ESCAPE)){
		glfwSetWindowShouldClose(vox::currentContext, true);
	}

	switch(kc_code){
	case 0:
		if(keyboard->keyJustUp(GLFW_KEY_UP)){
			kc_lastKey = GLFW_KEY_UP;
			kc_code += 1;
		}
		break;
	case 1:
		if(keyboard->keyJustUp(GLFW_KEY_UP)){
			if(kc_lastKey == GLFW_KEY_UP){
				kc_lastKey = GLFW_KEY_UP;
				kc_code += 1;
			}else{
				kc_code = 0;
			}
		}
		break;
	case 2:
		if(keyboard->keyJustUp(GLFW_KEY_DOWN)){
			if(kc_lastKey == GLFW_KEY_UP){
				kc_lastKey = GLFW_KEY_DOWN;
				kc_code += 1;
			}else{
				kc_code = 0;
			}
		}
		break;
	case 3:
		if(keyboard->keyJustUp(GLFW_KEY_DOWN)){
			if(kc_lastKey == GLFW_KEY_DOWN){
				kc_lastKey = GLFW_KEY_DOWN;
				kc_code += 1;
			}else{
				kc_code = 0;
			}
		}
		break;
	case 4:
		if(keyboard->keyJustUp(GLFW_KEY_LEFT)){
			if(kc_lastKey == GLFW_KEY_DOWN){
				kc_lastKey = GLFW_KEY_LEFT;
				kc_code += 1;
			}else{
				kc_code = 0;
			}
		}
		break;
	case 5:
		if(keyboard->keyJustUp(GLFW_KEY_RIGHT)){
			if(kc_lastKey == GLFW_KEY_LEFT){
				kc_lastKey = GLFW_KEY_RIGHT;
				kc_code += 1;
			}else{
				kc_code = 0;
			}
		}
		break;
	case 6:
		if(keyboard->keyJustUp(GLFW_KEY_LEFT)){
			if(kc_lastKey == GLFW_KEY_RIGHT){
				kc_lastKey = GLFW_KEY_LEFT;
				kc_code += 1;
			}else{
				kc_code = 0;
			}
		}
		break;
	case 7:
		if(keyboard->keyJustUp(GLFW_KEY_RIGHT)){
			if(kc_lastKey == GLFW_KEY_LEFT){
				kc_lastKey = GLFW_KEY_RIGHT;
				kc_code += 1;
			}else{
				kc_code = 0;
			}
		}
		break;
	case 8:
		if(keyboard->keyJustUp(GLFW_KEY_B)){
			if(kc_lastKey == GLFW_KEY_RIGHT){
				kc_lastKey = GLFW_KEY_B;
				kc_code += 1;
			}else{
				kc_code = 0;
			}
		}
		break;
	case 9:
		if(keyboard->keyJustUp(GLFW_KEY_A)){
			if(kc_lastKey == GLFW_KEY_B){
				kc_active = !kc_active;
			}
			kc_code = 0;
		}
		break;
	}
	
	if(currentScene != nullptr){
		currentScene->update(&vox::step);
	}
}

void Game::draw(void){
	vox::MatrixStack ms;
	VoxRenderOptions ro(nullptr, nullptr, nullptr);
	ro.kc_active = kc_active;
	currentScene->render(&ms, &ro);
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
		// If last printf() was more than 1 sec ago
		// printf and reset timer
		printf("%f FPS\n", double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}




void Game::switchScene(std::string _newScene){
	if(scenes.count(_newScene) > 0){
		switchingScene = true;
		newScene = _newScene;
	}else{
		switchingScene = false;
		newScene = "";
	}
}