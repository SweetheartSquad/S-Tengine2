// stb has its own guards, so do it before #pragma once
// the define is needed in one cpp before the include
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#pragma once

#include <Sweet.h>

#include <string>

#include <Keyboard.h>
#include <Mouse.h>

#include <OpenALSound.h>

#include <FileUtils.h>
#include <AntTweakBar.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Step sweet::step;
std::string sweet::title = "S-Tengine2";
Configuration sweet::config;

double sweet::lastTimestamp = 0;
double sweet::deltaTimeCorrection = 1;

bool sweet::antTweakBarInititialized = false;
bool sweet::drawAntTweakBar = false;

FT_Library sweet::freeTypeLibrary = nullptr;
GLFWwindow * sweet::currentContext = nullptr;


// Nvidia optiums fix to force discrete graphics
 extern "C" {  
      _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;  
 }  

void sweet::setGlfwWindowHints(){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void sweet::keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods){
	if(!sweet::antTweakBarInititialized || !TwEventKeyGLFW(_key, _action) ) {
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
}

void sweet::charCallback(GLFWwindow* _window, unsigned int _key) {
	if(sweet::antTweakBarInititialized) {
		TwEventCharGLFW(_key, GLFW_PRESS);
	}
}

void sweet::mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods){
	if(!sweet::antTweakBarInititialized || !TwEventMouseButtonGLFW(_button, _action) ) {
		Mouse * mouse = &Mouse::getInstance();
		if(_action == GLFW_PRESS){
			mouse->mouseDownListener(_button);
		}else if(_action == GLFW_RELEASE){
			mouse->mouseUpListener(_button);
		}
	}
}
void sweet::mousePositionCallback(GLFWwindow *_window, double _x, double _y){
	if(!sweet::antTweakBarInititialized || !TwEventMousePosGLFW(_x, _y) ) {
      	Mouse * mouse = &Mouse::getInstance();
		glm::uvec2 sd = getWindowDimensions();
		mouse->mousePositionListener(_x, sd.y - _y);
    }
}
void sweet::mouseScrollCallback(GLFWwindow *_window, double _x, double _y){
	if(!sweet::antTweakBarInititialized || !TwEventMouseWheelGLFW(_y) ) {
      	Mouse * mouse = &Mouse::getInstance();
		mouse->mouseWheelListener(_y);
    }
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
		if(antTweakBarInititialized) {
			if(drawAntTweakBar) {
				glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}else{
				glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}else {
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		glfwSetKeyCallback(_window, keyCallback);
		glfwSetMouseButtonCallback(_window, mouseButtonCallback);
		glfwSetCursorPosCallback(_window, mousePositionCallback);
		glfwSetScrollCallback(_window, mouseScrollCallback);
		Mouse::getInstance().active = true;
	}
}

void sweet::windowFocusCallback(GLFWwindow * _window, int _focused){
	std::stringstream ss;
	ss << _window << " focused: " << _focused;
	Log::info(ss.str());
	if(_focused == GL_TRUE){
		// gained focus
		glfwSetMouseButtonCallback(_window, attemptToActuallyRegainFocus);
	}else{
		// lost focus
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetKeyCallback(_window, nullptr);
		glfwSetMouseButtonCallback(_window, nullptr);
		glfwSetCursorPosCallback(_window, nullptr);
		glfwSetScrollCallback(_window, nullptr);
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
	
	if(!config.fullscreen){
		glfwSetWindowPos(window, 10, 50);
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#ifdef _DEBUG
	ST_LOG_INFO("Initializing AntTweakBar");
	sweet::initAntTweakBar(window);
#endif

	initializeInputCallbacks(window);

	return window;
}

void sweet::destructWindow(GLFWwindow * _window){
	glfwSetKeyCallback(_window, nullptr);
	glfwSetMouseButtonCallback(_window, nullptr);
	glfwSetCursorPosCallback(_window, nullptr);
	glfwSetScrollCallback(_window, nullptr);
	glfwSetWindowFocusCallback(_window, nullptr);
	glfwDestroyWindow(_window);

	if(antTweakBarInititialized) {
		TwTerminate();
		antTweakBarInititialized = false;
	}
}

void sweet::initialize(std::string _title){
	// you shouldn't be calling initialize if we've already got a window
	assert(sweet::currentContext == nullptr);

	// verify that the correct file structure is in place
	FileUtils::createDirectoryIfNotExists("data");
	FileUtils::createDirectoryIfNotExists("data/screenshots");
	FileUtils::createDirectoryIfNotExists("data/images");

	title = _title;

	sweet::setGlfwWindowHints();


	initOVR();



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

	// Initialize freetype
	if(FT_Init_FreeType(&freeTypeLibrary) != 0) {
		std::cerr << "Couldn't initialize FreeType library\n";
		throw;
	}


	// move mouse to the middle of the window
	int screenHeight, screenWidth;
	glfwGetWindowSize(sweet::currentContext, &screenWidth, &screenHeight);
	glfwSetCursorPos(sweet::currentContext, screenWidth/2, screenHeight/2);
	

	initOVR2();

	Log::info("*** Sweet Initialization ***");
}

void sweet::destruct(){

	destructWindow(currentContext);
	glfwTerminate();
	destructOVR();

	FT_Done_FreeType(freeTypeLibrary);

	NodeOpenAL::destruct();
	Log::info("*** Sweet Destruction ***");
}

void sweet::initAntTweakBar(GLFWwindow * _context) {
	glm::uvec2 dimens = sweet::getWindowDimensions(_context);

	TwInit(TW_OPENGL, NULL);
	TwWindowSize(dimens.x, dimens.y);

	glfwSetCharCallback(_context, charCallback);

	sweet::antTweakBarInititialized = true;
}

void sweet::toggleAntTweakBar() {
	if(sweet::antTweakBarInititialized){
		sweet::drawAntTweakBar = !sweet::drawAntTweakBar;
		if(drawAntTweakBar) {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}else {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

void sweet::initializeInputCallbacks(GLFWwindow * _context) {
	glfwSetKeyCallback(_context, keyCallback);
	glfwSetMouseButtonCallback(_context, mouseButtonCallback);
	glfwSetCursorPosCallback(_context, mousePositionCallback);
	glfwSetScrollCallback(_context, mouseScrollCallback);
	glfwSetWindowFocusCallback(_context, windowFocusCallback);
}

/////////// Delta Time Begin //////////////
void sweet::calculateDeltaTimeCorrection(){
	double targetFrameDuration = 1.0 / config.fps;
	double time = glfwGetTime();
	double deltaTime = time - lastTimestamp;
	deltaTimeCorrection = deltaTime/targetFrameDuration;
	while (deltaTimeCorrection > 1){
		deltaTime = targetFrameDuration;
		deltaTimeCorrection = 1;
	}
	lastTimestamp = time;

	step.targetFrameDuration = targetFrameDuration;
	step.time = time;
	step.setDeltaTime(deltaTime);
	step.deltaTimeCorrection = deltaTimeCorrection;
	step.lastTimestamp = lastTimestamp;
	step.cycles++;
}

unsigned long int sweet::getWindowWidth(){
	return getWindowWidth(currentContext);
}unsigned long int sweet::getWindowWidth(GLFWwindow * _window){
	return getWindowDimensions().y;
}

unsigned long int sweet::getWindowHeight(){
	return getWindowHeight(currentContext);
}
unsigned long int sweet::getWindowHeight(GLFWwindow * _window){
	return getWindowDimensions().x;
}

glm::uvec2 sweet::getWindowDimensions(){
	return getWindowDimensions(currentContext);
}glm::uvec2 sweet::getWindowDimensions(GLFWwindow * _window){
	int windowHeight;
	int windowWidth;

	glfwGetWindowSize(_window, &windowWidth, &windowHeight);

	return glm::uvec2(windowWidth, windowHeight);
}