#pragma once

#include <stdlib.h>

#include <Game.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <sweet.h>
#include <MatrixStack.h>
#include <VoxRenderOptions.h>
#include <GLUtils.h>

#include <Scene_Splash.h>
#include <Scene_Splash_Engine.h>
#include <Scene_Splash_SweetHeartSquad.h>
#include <MeshInterface.h>
#include <Log.h>
#include <scenario/Scenario.h>

// for screenshots
#include <ctime>
#include <stb/stb_image_write.h>
#include <AntTweakBar.h>

Game::Game(std::string _firstSceneKey, Scene * _firstScene, bool _isRunning) :
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
	autoResize(true),
	numSplashScenes(0)
{
	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
	viewPortWidth = width;
	viewPortHeight = height;
	viewPortX = 0;
	viewPortY = 0;
	lastTime = glfwGetTime();
	nbFrames = 0;
	
	scenes.insert(std::pair<std::string, Scene *>(_firstSceneKey, _firstScene));
	currentScene = _firstScene;
	currentSceneKey = _firstSceneKey;
}

void Game::addSplashes(){
	addSplash(new Scene_Splash_Engine(this));
	addSplash(new Scene_Splash_SweetHeartSquad(this));
}

void Game::init(){
	addSplashes();
	if(numSplashScenes > 0){
		std::stringstream ss;
		ss << "SCENE_SPLASH" << numSplashScenes;
		Scene_Splash * lastSplash = dynamic_cast<Scene_Splash *>(scenes[ss.str()]);
		lastSplash->nextScene = currentSceneKey;
		currentSceneKey = "SCENE_SPLASH1";
	}else{
		currentSceneKey = currentSceneKey;
	}
	currentScene = scenes[currentSceneKey];
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
	if(accumulator >= sweet::step.targetFrameDuration){
		accumulator -= sweet::step.targetFrameDuration;
#endif
		sweet::calculateDeltaTimeCorrection();
		glfwPollEvents();
		update(&sweet::step);
		draw();

		glfwSwapBuffers(sweet::currentContext);
		manageInput();
		isRunning = !glfwWindowShouldClose(sweet::currentContext);

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
	
	if(currentScene != nullptr/* && sweet::step.deltaTimeCorrection < 5*/){
		currentScene->update(_step);
	}
}

void Game::draw(void){
	if(autoResize){
		resize();
	}
	sweet::MatrixStack ms;
	VoxRenderOptions ro(nullptr, nullptr, nullptr);
	ro.kc_active = kc_active;
	if(currentScene != nullptr){
		currentScene->render(&ms, &ro);
	}
	if(sweet::drawAntTweakBar && sweet::antTweakBarInititialized) {
		TwDraw(); 
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
	std::stringstream ss;
	if(scenes.count(_newSceneKey) > 0){
		switchingScene = true;
		newSceneKey = _newSceneKey;
		ss << "newSceneKey: " << newSceneKey;
		Log::info(ss.str());
	}else{
		ss << "Scene \"" << _newSceneKey << "\" does not exist, newSceneKey is now null";
		switchingScene = false;
		newSceneKey = "";
		Log::warn(ss.str());
	}
}

void Game::addSplash(Scene_Splash * _splashScene){
	std::stringstream ss1;
	ss1 << "SCENE_SPLASH" << numSplashScenes;
	++numSplashScenes;
	std::stringstream ss2;
	ss2 << "SCENE_SPLASH" << numSplashScenes;
	if(numSplashScenes > 1){
		dynamic_cast<Scene_Splash *>(scenes[ss1.str()])->nextScene = ss2.str();
	}
	
	auto res = scenes.insert(std::pair<std::string, Scene * >(ss2.str(), _splashScene));
	if(!res.second){
		Log::error("Scene with name\"" + ss2.str() + "\" already exists, splash scene could not be inserted.");
	}
}


void Game::load(){
	Transform::transformShader->load();
	Transform::transformIndicator->load();
	Transform::transformIndicator->configureDefaultVertexAttributes(Transform::transformShader);
	
	for(std::pair<std::string, Scene *> s : scenes){
		s.second->load();
	}
	ResourceManager::load();
}

void Game::unload(){
	ResourceManager::unload();
	for(std::pair<std::string, Scene *> s : scenes){
		s.second->unload();
	}
	Transform::transformIndicator->unload();
	Transform::transformShader->unload();
}

void Game::toggleFullScreen(){
	// unload everything
	unload();

	// destroy the current window
	sweet::destructWindow(sweet::currentContext);

	// Toggle fullscreen flag.
	sweet::config.fullscreen = !sweet::config.fullscreen;
#ifdef _DEBUG
	sweet::config.fullscreen = false;
#endif

	// create new window and make assign it to the current context
	sweet::currentContext = sweet::initWindow();
	glfwMakeContextCurrent(sweet::currentContext);
	
	if(autoResize){
		resize();
	}
	
	// reload everything
	load();
	checkForGlError(false);
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
	glfwSetWindowShouldClose(sweet::currentContext, true);
}

void Game::resize(){
	glm::vec2 screenDimensions = sweet::getWindowDimensions();
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