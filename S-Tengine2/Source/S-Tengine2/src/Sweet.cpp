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
#include "scenario/Scenario.h"
#include "NodeUI.h"
#include "shader/ComponentShaderBase.h"
#include "AnimationRig.h"

#include <FileUtils.h>
#include <NumberUtils.h>

#include <AntTweakBar.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Step sweet::step;
std::string sweet::title = "S-Tengine2";
Configuration sweet::config;

double sweet::lastTimestamp = 0;
double sweet::deltaTimeCorrection = 1;
bool sweet::focused = true;

bool sweet::antTweakBarInititialized = false;
bool sweet::drawAntTweakBar = false;

int sweet::cursorMode = GLFW_CURSOR_DISABLED;

FT_Library sweet::freeTypeLibrary = nullptr;
GLFWwindow * sweet::currentContext = nullptr;
GLFWmonitor * sweet::currentMonitor = nullptr;

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
			sweet::focused = true;
			if(antTweakBarInititialized) {
				if(drawAntTweakBar) {
					glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}else{
					glfwSetInputMode(_window, GLFW_CURSOR, cursorMode);
				}
			}else {
				glfwSetInputMode(_window, GLFW_CURSOR, cursorMode);
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
		sweet::focused = false;
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

void sweet::printNodes() {
#ifdef _DEBUG
	if(Node::nodeCounting){
		std::cout << "Final node count: " << Node::nodes.size() << std::endl;

		for(auto n : Node::nodes){
			std::cout << typeid(*n).name() << " " << n << " -- Name: " << n->name << std::endl;
		}
	}

#endif
}

GLFWwindow * sweet::initWindow(){
	glm::ivec2 target(config.resolution.x, config.resolution.y);
	bool fullscreen = config.fullscreen;

	// if we're rendering to an oculus, match the resolution target to the HMD output size
	// also prevent fullscreen windows
	if(sweet::ovrInitialized){
		target.x = sweet::hmdDesc.Resolution.w;
		target.y = sweet::hmdDesc.Resolution.h;
		fullscreen = false;
	}

	// if it's a fullscreen window, we need to use one of the monitor's available video modes
	// find the best match between the target resolution and the available video modes
	if(fullscreen){
		int numVideoModes = 0;
		const GLFWvidmode * videoModes = glfwGetVideoModes(currentMonitor, &numVideoModes);
		int diff = INT_MAX;
		int bestMatch = 0;
		for(unsigned long int i = 0; i < numVideoModes; ++i){
			int d = (target.x - videoModes[i].width) + (target.y - videoModes[i].height);
			if(d < diff){
				bestMatch = i;
				diff = d;
			}
		}
		target.x = videoModes[bestMatch].width;
		target.y = videoModes[bestMatch].height;
	}

	GLFWwindow * window = glfwCreateWindow(target.x, target.y, title.c_str(), fullscreen ? currentMonitor : nullptr, nullptr);

	if (!window){
		glfwTerminate();
		throw "some sort of window error?";
	}

	// if we're running in windowed mode, center the window in the current monitor
	if(!fullscreen){
		int xPos = 0, yPos = 0;
		glfwGetMonitorPos(currentMonitor, &xPos, &yPos);
		const GLFWvidmode* mode = glfwGetVideoMode(currentMonitor);
		glfwSetWindowPos(window, xPos + (mode->width - target.x)/2, yPos + (mode->height - target.y)/2);
	}

	// TODO: make this based on config too
	glfwSetInputMode(window, GLFW_CURSOR, cursorMode);

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

	// seed RNG
	sweet::NumberUtils::seed(config.rngSeed);

	// set the audio gain
	for(auto g : config.gain){
		if(g.first == "master"){
			OpenAL_Sound::masterGain = g.second;
		}else{
			OpenAL_Sound::categoricalGain[g.first] = g.second;
		}
	}

	// get the monitor
	int numMonitors = 0;
	GLFWmonitor ** monitors = glfwGetMonitors(&numMonitors);
	if(numMonitors == 0 || monitors == nullptr){
		throw "monitor error";
	}
	if(numMonitors-1 < config.monitor){
		config.monitor = numMonitors-1;
	}
	currentMonitor = monitors[config.monitor];

	// create the application window
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

	Log::info("*** Sweet Initialization ***");
}

void sweet::destruct(){
	// get rid of static assets
	Scenario::destruct();
	NodeUI::bgShader->decrementAndDelete();
	NodeUI::colliderMesh->decrementAndDelete();
	Transform::transformShader->decrementAndDelete();
	Transform::transformIndicator->decrementAndDelete();
	if(AnimationJoint::shaderVis != nullptr){
		AnimationJoint::shaderVis->decrementAndDelete();
	}

	// get rid of window and OpenGL context
	destructWindow(currentContext);
	glfwTerminate();

	// uninitialize libraries
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
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, cursorMode);
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

unsigned long int sweet::getWindowWidth(GLFWwindow * _window){
	if(_window == nullptr){
		_window = currentContext;
	}
	return getWindowDimensions().x;
}

unsigned long int sweet::getWindowHeight(GLFWwindow * _window){
	if(_window == nullptr){
		_window = currentContext;
	}
	return getWindowDimensions().y;
}

glm::uvec2 sweet::getWindowDimensions(GLFWwindow * _window){
	if(_window == nullptr){
		_window = currentContext;
	}
	int windowHeight;
	int windowWidth;

	glfwGetWindowSize(_window, &windowWidth, &windowHeight);

	return glm::uvec2(windowWidth, windowHeight);
}

float sweet::getDpi(GLFWmonitor * _monitor){
	if(_monitor == nullptr){
		_monitor = currentMonitor;
	}
	int widthMM, heightMM;
	glfwGetMonitorPhysicalSize(_monitor, &widthMM, &heightMM);
	float res = (float) glfwGetVideoMode(_monitor)->width / (widthMM / 25.4f);
	return res;
}

void sweet::setCursorMode(int _mode) {
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, _mode);
	cursorMode = _mode;
}