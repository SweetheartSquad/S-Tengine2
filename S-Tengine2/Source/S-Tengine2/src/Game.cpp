#pragma once

#include <stdlib.h>

#include <Game.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <sweet.h>
#include <MatrixStack.h>
#include <RenderOptions.h>

#include <Scene_Splash_Engine.h>
#include <Scene_Splash_SweetHeartSquad.h>
#include <MeshInterface.h>
#include <Log.h>
#include <scenario/Scenario.h>

// for screenshots
#include <DateUtils.h>
#include <ProgrammaticTexture.h>

#include <AntTweakBar.h>

#include <GLFW/glfw3.h>


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

		if(switchingScene){
			if(deleteOldScene){
				delete currentScene;
				scenes.erase(currentSceneKey);
			}
			currentSceneKey = newSceneKey;
			currentScene = scenes.at(currentSceneKey);
			switchingScene = false;
			newSceneKey = "";
		}else{
			draw();
			manageInput();
			isRunning = !glfwWindowShouldClose(sweet::currentContext);
		}
#ifdef VOX_LIMIT_FRAMERATE
	}
#endif
}

void Game::update(Step * _step){

	if(printFPS){
		printFps();
	}
	if(sweet::focused){
		if(currentScene != nullptr/* && sweet::step.deltaTimeCorrection < 5*/){
			currentScene->update(_step);
		}
	}
}

void Game::draw(Scene * _scene){
	int width, height;
	glfwGetFramebufferSize(sweet::currentContext, &width, &height);
	setViewport(0, 0, width, height);
	if(width <= 0 || height <= 0) {
		ST_LOG_WARN("Window size 0 -- Draw skipped");
		return;
	}
	if(_scene == nullptr){
		_scene = currentScene;
	}
	if(autoResize){
		resize();
	}
	sweet::MatrixStack ms;
	RenderOptions ro(nullptr, nullptr, nullptr);
	if(_scene != nullptr){
		ro.lights = &_scene->lights;
		for(auto s : Shader::allShaders){
			ComponentShaderBase * sb = dynamic_cast<ComponentShaderBase *>(s);
			if(sb != nullptr){
				sb->lightingDirty = true;
			}
			if(s->bindShader()){
				ro.shader = s;
				s->clean(&ms, &ro, nullptr);
				checkForGlError(false);
			}
		}
		ro.shader = nullptr;
		_scene->render(&ms, &ro);
	}
	if(sweet::drawAntTweakBar && sweet::antTweakBarInititialized) {
		TwDraw(); 
	}
	glfwSwapBuffers(sweet::currentContext);
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
		
	filepath << "../screenshots/" << sweet::DateUtils::getDatetime() << ".tga";
	ProgrammaticTexture * tex = new ProgrammaticTexture();
	tex->allocate(viewPortWidth, viewPortHeight, 4);
	glReadPixels(0, 0, viewPortWidth, viewPortHeight, GL_RGBA, GL_UNSIGNED_BYTE, tex->data);
	tex->saveImageData(filepath.str());
	delete tex;

	Log::info("Screenshot \""+filepath.str()+"\" saved");
}

void Game::exit(){
	glfwSetWindowShouldClose(sweet::currentContext, true);
}

void Game::resize(){
	glm::uvec2 screenDimensions = sweet::getWindowDimensions();
	setViewport(0, 0, screenDimensions.x, screenDimensions.y);
}

void Game::setViewport(int _x, int _y, int _w, int _h){
	viewPortX = _x;
	viewPortY = _y;
	viewPortWidth = _w;
	viewPortHeight = _h;

	glViewport(viewPortX, viewPortY, viewPortWidth, viewPortHeight);
	glScissor(viewPortX, viewPortY, viewPortWidth, viewPortHeight);
}