// stb has its own guards, so do it before #pragma once
// the define is needed in one cpp before the include
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#pragma once

#include "Sweet.h"

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Keyboard.h"
#include "Mouse.h"

#include <OpenALSound.h>
#include <System.h>

#include <FileUtils.h>

Step sweet::step;
std::string sweet::title = "S-Tengine2";
Configuration sweet::config;

double sweet::lastTimestamp = 0;
double sweet::deltaTimeCorrection = 1;

bool sweet::fullscreen = false;

FT_Library sweet::freeTypeLibrary = nullptr;
GLFWwindow * sweet::currentContext = nullptr;

void sweet::setGlfwWindowHints(){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void sweet::keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods){
	Keyboard * keyboard = &Keyboard::getInstance();
	keyboard->alt = (_mods & GLFW_MOD_ALT) != 0;
	keyboard->shift = (_mods & GLFW_MOD_SHIFT) != 0;
	keyboard->control = (_mods & GLFW_MOD_CONTROL) != 0;
	keyboard->super = (_mods & GLFW_MOD_SUPER) != 0;
	if(_action == GLFW_PRESS){
		keyboard->keyDownListener(_key);
	}else if(_action == GLFW_RELEASE){
		keyboard->keyUpListener(_key);
	}
}
void sweet::mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods){
	Mouse * mouse = &Mouse::getInstance();
	if(_action == GLFW_PRESS){
		mouse->mouseDownListener(_button);
	}else if(_action == GLFW_RELEASE){
		mouse->mouseUpListener(_button);
	}
}
void sweet::mousePositionCallback(GLFWwindow *_window, double _x, double _y){
	Mouse * mouse = &Mouse::getInstance();
	glm::uvec2 sd = getScreenDimensions();
	mouse->mousePositionListener(_x, sd.y - _y);
}

void sweet::attemptToActuallyRegainFocus(GLFWwindow *_window, int _button, int _action, int _mods){
	// grab the mouse coordinates and the screen size
	int w, h;
	double x, y;
	glfwGetWindowSize(_window, &w, &h);
	glfwGetCursorPos(_window, &x, &y);

	// only re-attach the event listeners and set the mouse mode if the mouse was pressed while within the screen bounds
	// this prevents mouse events which took place on the window border from re-capturing the mouse
	if(x >= 0 && x <= w
		&& y >= 0 && y <= h){
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetKeyCallback(_window, keyCallback);
		glfwSetMouseButtonCallback(_window, mouseButtonCallback);
		glfwSetCursorPosCallback(_window, mousePositionCallback);
		Mouse::getInstance().active = true;
	}
}

void sweet::windowFocusCallback(GLFWwindow * _window, int _focused){
	std::cout << _window << " focused: " << _focused << std::endl;
	if(_focused == GL_TRUE){
		// gained focus
		glfwSetMouseButtonCallback(_window, attemptToActuallyRegainFocus);
	}else{
		// lost focus
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetKeyCallback(_window, nullptr);
		glfwSetMouseButtonCallback(_window, nullptr);
		glfwSetCursorPosCallback(_window, nullptr);
		Mouse::getInstance().active = false;
	}
}

void sweet::error_callback(int _error, const char * _description){
	fputs(_description, stderr);
}

GLFWwindow * sweet::initWindow(){
	GLFWwindow * window;
	window = glfwCreateWindow(config.resolution.x, config.resolution.y, title.c_str(), config.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

	if (!window){
		glfwTerminate();
		throw "some sort of window error?";
	}
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mousePositionCallback);
	glfwSetWindowFocusCallback(window, windowFocusCallback);

	glfwSetWindowPos(window, 10, 50);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

void sweet::initialize(std::string _title){
	// you shouldn't be calling initialize if we've already got a window
	assert(sweet::currentContext == nullptr);

	// verify that the correct file structure is in place
	FileUtils::createDirectoryIfNotExists("data");
	FileUtils::createDirectoryIfNotExists("data/screenshots");
	FileUtils::createDirectoryIfNotExists("data/images");

	title = _title;
	step.targetFrameDuration = 0.1667;

	sweet::setGlfwWindowHints();

	// initialize glfw
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()){
		exit(EXIT_FAILURE);
	}
	
	// load configuration file
	config.load("data/config.json");
	
	
	sweet::currentContext = sweet::initWindow();
	glfwMakeContextCurrent(sweet::currentContext);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK){
		// Problem: glewInit failed, something is seriously wrong.
		std::cerr << "ERROR: " << glewGetErrorString(err) << std::endl;
		throw;
	}

	// move mouse to the middle of the window
	int screenHeight, screenWidth;
	glfwGetWindowSize(sweet::currentContext, &screenWidth, &screenHeight);
	glfwSetCursorPos(sweet::currentContext, screenWidth/2, screenHeight/2);

	// Initialize freetype
	if(FT_Init_FreeType(&freeTypeLibrary) != 0) {
		std::cerr << "Couldn't initialize FreeType library\n";
		throw;
	}
	Log::info("*** Sweet Initialization ***");
}

void sweet::destruct(){
	glfwTerminate();

	FT_Done_FreeType(freeTypeLibrary);
	
	NodeOpenAL::destruct();
	Log::info("*** Sweet Destruction ***");
}

/////////// Delta Time Begin //////////////
void sweet::calculateDeltaTimeCorrection(){
	double targetFrameDuration = static_cast<double>(1) / FPS;
	double time = glfwGetTime();
	double deltaTime = time - lastTimestamp;
	deltaTimeCorrection = deltaTime/targetFrameDuration;
	while (deltaTimeCorrection > 1){
		deltaTime = targetFrameDuration;
		deltaTimeCorrection = 1;
	}
	lastTimestamp = time;

	sweet::step.targetFrameDuration = targetFrameDuration;
	sweet::step.time = time;
	sweet::step.setDeltaTime(deltaTime);
	sweet::step.deltaTimeCorrection = deltaTimeCorrection;
	sweet::step.lastTimestamp = lastTimestamp;
}
