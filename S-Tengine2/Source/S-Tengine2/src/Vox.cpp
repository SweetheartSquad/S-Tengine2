// stb has its own guards, so do it before #pragma once
// the define is needed in one cpp before the include
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#pragma once

#include "Vox.h"

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Keyboard.h"
#include "Mouse.h"

#include <OpenALSound.h>
#include <System.h>

#include <FileUtils.h>

Step vox::step;
std::string vox::title = "S-Tengine2";

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
static void mousePositionCallback(GLFWwindow *_window, double _x, double _y){
	Mouse * mouse = &Mouse::getInstance();
	glm::uvec2 sd = vox::getScreenDimensions();
	mouse->mousePositionListener(_x, sd.y - _y);
}

static void attemptToActuallyRegainFocus(GLFWwindow *_window, int _button, int _action, int _mods){
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

static void windowFocusCallback(GLFWwindow * _window, int _focused){
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

static void error_callback(int _error, const char * _description){
	fputs(_description, stderr);
}

void vox::initWindow(GLFWwindow * _w){
	if(_w != nullptr){
		//glfwMakeContextCurrent(currentContext);
		glfwSetKeyCallback(_w, keyCallback);
		glfwSetMouseButtonCallback(_w, mouseButtonCallback);
		glfwSetCursorPosCallback(_w, mousePositionCallback);
		glfwSetInputMode(_w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetWindowFocusCallback(_w, windowFocusCallback);
	}
}

void vox::initialize(std::string _title){
	// verify that the correct file structure is in place
	FileUtils::createDirectoryIfNotExists("data");
	FileUtils::createDirectoryIfNotExists("data/screenshots");
	FileUtils::createDirectoryIfNotExists("data/images");

	title = _title;
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
	window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
#else
	window = glfwCreateWindow(w, h, title.c_str(), vox::fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
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

	// Initialize freetype
	if(FT_Init_FreeType(&freeTypeLibrary) != 0) {
		std::cerr << "Couldn't initialize FreeType library\n";
		throw;
	}
}

void vox::destruct(){
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
