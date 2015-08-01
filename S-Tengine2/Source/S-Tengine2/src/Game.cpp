#pragma once

#include <stdlib.h>

#include <Game.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <Vox.h>
#include <MatrixStack.h>
#include <VoxRenderOptions.h>
#include <GLUtils.h>
#include <System.h>

#include <SceneSplash.h>
#include <SceneSweetheartSquad.h>
#include <MeshInterface.h>
#include <Log.h>

// for screenshots
#include <ctime>
#include <stb/stb_image_write.h>

Game::Game(bool _isRunning, std::pair<std::string, Scene *> _firstScene, bool _splashScreen) :
	splashScreen(_splashScreen),
	accumulator(0.0),
	lastTimestep(0.0),
	mouse(&Mouse::getInstance()),
	keyboard(&Keyboard::getInstance()),
	isRunning(_isRunning),
	printFPS(true),
	currentScene(nullptr),
	currentSceneKey(""),
	switchingScene(false),
	newSceneKey(""),
	deleteOldScene(false),
	kc_lastKey(0),
	kc_code(0),
	kc_active(false),
	kc_just_active(false),
	autoResize(true)
{
	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	viewPortWidth = width;
	viewPortHeight = height;
	viewPortX = 0;
	viewPortY = 0;
	lastTime = glfwGetTime();
	nbFrames = 0;
	
	scenes.insert(_firstScene);
	if(splashScreen){
		currentSceneKey = "S-TENGINE2_SPLASH";
		SceneSplash * ss = new SceneSplash(this);
		SceneSweetheartSquad * sss = new SceneSweetheartSquad(this);
		ss->nextScene = "SWEETHEARTSQUAD_SPLASH";
		sss->nextScene = _firstScene.first;
		currentScene = ss;
		scenes.insert(std::pair<std::string, Scene * >(currentSceneKey, ss));
		scenes.insert(std::pair<std::string, Scene * >("SWEETHEARTSQUAD_SPLASH", sss));
	}else{
		currentSceneKey = _firstScene.first;
		currentScene = _firstScene.second;
	}
}

Game::~Game(void){
	for(auto scene : scenes){
		delete scene.second;
	}
	scenes.clear();
}

void Game::performGameLoop(){
#ifdef VOX_LIMIT_FRAMERATE
	double time = glfwGetTime();
	accumulator += time - lastTimestep;
	lastTimestep = time;
	//std::cout << accumulator << std::endl;
	if(accumulator >= vox::step.targetFrameDuration){
		accumulator -= vox::step.targetFrameDuration;
#endif
		vox::calculateDeltaTimeCorrection();
		glfwPollEvents();
		update(&vox::step);
		draw();

		glfwSwapBuffers(vox::currentContext);
		manageInput();
		isRunning = !glfwWindowShouldClose(vox::currentContext);

		if(switchingScene){
			if(deleteOldScene){
				scenes.erase(currentSceneKey);
				delete currentScene;
			}
			currentSceneKey = newSceneKey;
			currentScene = scenes.at(currentSceneKey);
			switchingScene = false;
			newSceneKey = "";
		}	
#ifdef VOX_LIMIT_FRAMERATE
	}
#endif
}

void Game::update(Step * _step){

	if(printFPS){
		printFps();
	}
	if(kc_just_active){
		kc_just_active = false;
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
				kc_just_active = true;
			}
			kc_code = 0;
		}
		break;
	}
	
	if(currentScene != nullptr/* && vox::step.deltaTimeCorrection < 5*/){
		currentScene->update(_step);
	}
}

void Game::draw(void){
	if(autoResize){
		resize();
	}
	vox::MatrixStack ms;
	VoxRenderOptions ro(nullptr, nullptr, nullptr);
	ro.kc_active = kc_active;
	if(currentScene != nullptr){
		currentScene->render(&ms, &ro);
	}
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
		std::stringstream ss;
		ss << nbFrames << "FPS";
		Log::info(ss.str());
		nbFrames = 0;
		lastTime += 1.0;
	}
}

void Game::switchScene(std::string _newSceneKey, bool _deleteOldScene){
	deleteOldScene = _deleteOldScene;
	if(scenes.count(_newSceneKey) > 0){
		switchingScene = true;
		newSceneKey = _newSceneKey;
	}else{
		switchingScene = false;
		newSceneKey = "";
	}
	Log::info("newSceneKey: " + newSceneKey);
}




void Game::toggleFullScreen(){
	// unload everything
	ResourceManager::unload();
	for(std::pair<std::string, Scene *> s : scenes){
		s.second->unload();
	}
	Transform::transformIndicator->unload();
	Transform::transformShader->unload();
	
	// destroy the current window
	glfwSetKeyCallback(vox::currentContext, nullptr);
	glfwSetMouseButtonCallback(vox::currentContext, nullptr);
	glfwSetCursorPosCallback(vox::currentContext, nullptr);
	glfwSetWindowFocusCallback(vox::currentContext, nullptr);
	glfwDestroyWindow(vox::currentContext);

	// Toggle fullscreen flag.
	vox::fullscreen = !vox::fullscreen;

	// create new window and make assign it to the current context
	vox::currentContext = vox::initWindow();
	glfwMakeContextCurrent(vox::currentContext);
	
	if(autoResize){
		resize();
	}
	
	// reload everything
	Transform::transformShader->load();
	Transform::transformIndicator->load();
	Transform::transformIndicator->configureDefaultVertexAttributes(Transform::transformShader);
	
	for(std::pair<std::string, Scene *> s : scenes){
		s.second->load();
	}
	ResourceManager::load();

	checkForGlError(0,__FILE__,__LINE__);
}


void Game::takeScreenshot(){
	std::stringstream filepath;
		
	// create a string with the format "../screenshots/YYYY-MM-DD_TTTTTTTTTT.tga"
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	filepath
		<< "data/screenshots/"
		<< (now.tm_year + 1900)
		<< '-'
		<< (now.tm_mon + 1)
		<< '-'
		<< now.tm_mday
		<< '_'
		<< t
		<< ".tga";
	GLubyte * data = (GLubyte *)malloc(sizeof(GLubyte) * viewPortWidth * viewPortHeight * 4);
	glReadPixels(0, 0, viewPortWidth, viewPortHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);

	if(stbi_write_tga(filepath.str().c_str(), viewPortWidth, viewPortHeight, 4, data, 1)){
		Log::info("Screenshot \""+filepath.str()+"\" saved");
	}else{
		Log::error("Screenshot \""+filepath.str()+"\" not saved");
	}
	
	free(data);
}

void Game::exit(){
	glfwSetWindowShouldClose(vox::currentContext, true);
}

void Game::resize(){
	glm::vec2 screenDimensions = vox::getScreenDimensions();
	setViewport(0, 0, screenDimensions.x, screenDimensions.y);
}

void Game::setViewport(float _x, float _y, float _w, float _h){
	viewPortX = _x;
	viewPortY = _y;
	viewPortWidth = _w;
	viewPortHeight = _h;

	glViewport(viewPortX, viewPortY, viewPortWidth, viewPortHeight);
	glScissor(viewPortX, viewPortY, viewPortWidth, viewPortHeight);
}