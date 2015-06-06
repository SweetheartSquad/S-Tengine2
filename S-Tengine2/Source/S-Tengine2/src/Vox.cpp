#pragma once

#include "Vox.h"

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Keyboard.h"
#include "Mouse.h"

#include <OpenALSound.h>

Step vox::step;

double vox::lastTimestamp = 0;
double vox::deltaTimeCorrection = 1;

bool vox::fullscreen = false;

FT_Library vox::freeTypeLibrary = nullptr;

GLFWwindow * vox::currentContext = nullptr;

void vox::setGlfwWindowHints(){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods){
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
static void mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods){
	Mouse * mouse = &Mouse::getInstance();
	if(_action == GLFW_PRESS){
		mouse->mouseDownListener(_button);
	}else if(_action == GLFW_RELEASE){
		mouse->mouseUpListener(_button);
	}
}
static void mousePostionCallback(GLFWwindow *_window, double _x, double _y){
	Mouse * mouse = &Mouse::getInstance();
	mouse->mousePositionListener(_x, _y);
}

static void error_callback(int _error, const char * _description){
	fputs(_description, stderr);
}

void vox::initWindow(GLFWwindow * _w){
	if(_w != nullptr){
		//glfwMakeContextCurrent(currentContext);
		glfwSetKeyCallback(_w, keyCallback);
		glfwSetMouseButtonCallback(_w, mouseButtonCallback);
		glfwSetCursorPosCallback(_w, mousePostionCallback);
		glfwSetInputMode(_w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void vox::initialize(std::string _title){

	step.targetFrameDuration = 0.1667;

	vox::setGlfwWindowHints();

	GLFWwindow * window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()){
		exit(EXIT_FAILURE);
	}

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	int w = mode->width;
	int	h = mode->height;

#ifdef _DEBUG
	w = mode->width/2;
	h = mode->height/2;
	window = glfwCreateWindow(w, h, _title.c_str(), nullptr, nullptr);
#else
	window = glfwCreateWindow(w, h, _title.c_str(), vox::fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
#endif

	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	vox::initWindow(window);
	glfwMakeContextCurrent(window);
	vox::currentContext = window;

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK){
		// Problem: glewInit failed, something is seriously wrong.
		std::cerr << "ERROR: " << glewGetErrorString(err) << std::endl;
		throw;
	}

	int screenHeight;
	int screenWidth;

	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	glfwSetCursorPos(window, screenWidth/2, screenHeight/2);

	//Initialize freetype
	if(FT_Init_FreeType(&freeTypeLibrary) != 0) {
		std::cerr << "Couldn't initialize FreeType library\n";
		throw;
	}
}

void vox::destruct(){
	glfwDestroyWindow(vox::currentContext);
	glfwTerminate();

	FT_Done_FreeType(freeTypeLibrary);
	
	NodeOpenAL::destruct();
}

/////////// Delta Time Begin //////////////
void vox::calculateDeltaTimeCorrection(){
	double targetFrameDuration = static_cast<double>(1) / FPS;
	double time = glfwGetTime();
	double deltaTime = time - lastTimestamp;
	deltaTimeCorrection = deltaTime/targetFrameDuration;
	while (deltaTimeCorrection > 1){
		deltaTime = targetFrameDuration;
		deltaTimeCorrection = 1;
	}
	lastTimestamp = time;

	vox::step.targetFrameDuration = targetFrameDuration;
	vox::step.time = time;
	vox::step.setDeltaTime(deltaTime);
	vox::step.deltaTimeCorrection = deltaTimeCorrection;
	vox::step.lastTimestamp = lastTimestamp;
}
