#pragma once

#ifndef FPS
#define FPS 60
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

#include "MatrixStack.h"
#include <Configuration.h>
#include "Step.h"

// oculus support
#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define OVR_OS_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#define OVR_OS_MAC
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#define OVR_OS_LINUX
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <OVR_CAPI_GL.h>

#include <Log.h>

#ifdef _DEBUG
// OVR error-checking macro (enabled because _DEBUG is defined)
// If an error is found, it is printed to the console and the application will fail an assertion
#define checkForOvrError(_ovrFunc) do{\
	ovrResult result = (_ovrFunc);\
	if(OVR_FAILURE(result)) {\
	ovrErrorInfo errorInfo;\
	ovr_GetLastErrorInfo(&errorInfo);\
	Log::warn("OVR failed: " + std::string(errorInfo.ErrorString));\
	}\
}while(false);
#else
// OVR error-checking macro (disabled because _DEBUG is not defined)
#define checkForOvrError(_ovrFunc) (_ovrFunc)
#endif

namespace sweet{
	extern Configuration config;
	extern Step step;
	extern std::string title;

	/** Handling for the cursor - Defaults to disabled*/
	extern int cursorMode;

	/** Equal to glfwGetTime() the last time the delta time correction was calculated */
	extern double lastTimestamp;

	/**
	* Multiply by this to correct for differences in framerate
	*/
	extern double deltaTimeCorrection;

	/**
	* Stores a reference to the main window
	*/
	extern GLFWwindow * currentContext;

	// A refernce to the monitor which the application treats as primary
	extern GLFWmonitor * currentMonitor;

	// whether the application window is currently focused
	extern bool focused;

	/**
	* Stores a reference to the global freetype instance
	*/
	extern FT_Library freeTypeLibrary;

	/**
	* Whether AntTweakBar has been initialized or not
	*/
	extern bool antTweakBarInititialized;

	/**
	* Whether AntTweakBar should be drawn by game or not
	*/
	extern bool drawAntTweakBar;

	/**
	* Sets the window properties that need to be initialized before window creation
	*/
	void setGlfwWindowHints();

	/**
	*
	*/
	GLFWwindow * initWindow();
	/**
	*
	*/
	void destructWindow(GLFWwindow * _window);

	/**
	* Initializes everything need to get the engine up and running
	* Should be the first S-Tengine2 call made
	*/
	void initialize();

	/**
	* Calculates the delta time and delta time correction and sets them in step
	*/
	void calculateDeltaTimeCorrection();

	/**
	* Destroys the opengl context, shuts down GLFW and closes the application
	*/
	void destruct();

	/**
	* Intitializes ant tweak bar
	*/
	void initAntTweakBar(GLFWwindow * _context);

	/**
	* Flips drawAntTweakBar
	* Sets the cursor mode so that ant tweak bar works properly
	*/
	void toggleAntTweakBar();

	// OVR head-mounted display
	extern ovrHmd * hmd;
	extern ovrHmdDesc hmdDesc;
	extern ovrGraphicsLuid luid;
	extern bool ovrInitialized;
	// Initializes OVR for oculus support
	void initOVR();
	// De-initializes OVR
	void destructOVR();

	void initializeInputCallbacks(GLFWwindow * _context);
	void setGlfwWindowHints();
	void keyCallback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mods);
	void charCallback(GLFWwindow * _window, unsigned int _key);
	void mouseButtonCallback(GLFWwindow * _window, int _button, int _action, int _mods);
	void mousePositionCallback(GLFWwindow *_window, double _x, double _y);
	void mouseScrollCallback(GLFWwindow *_window, double _x, double _y);
	void attemptToActuallyRegainFocus(GLFWwindow *_window, int _button, int _action, int _mods);
	void windowFocusCallback(GLFWwindow * _window, int _focused);
	void error_callback(int _error, const char * _description);

	// if in debug mode and node counting is turned on, prints every currently existing node in the format: TYPE ADDRESS NAME
	void printNodes();

	/**
	* @returns the width of _window in pixels
	* if _window is nullptr, uses the sweet::currentContext as the window
	*/
	unsigned long int getWindowWidth(GLFWwindow * _window = nullptr);

	/**
	* @returns the height of _window in pixels
	* if _window is nullptr, uses the sweet::currentContext as the window
	*/
	unsigned long int getWindowHeight(GLFWwindow * _window = nullptr);

	/**
	* @returns the width and height of _window in pixels as a Dimension object
	* if _window is nullptr, uses the sweet::currentContext as the window
	*/
	glm::uvec2 getWindowDimensions(GLFWwindow * _window = nullptr);

	/*
	* @returns the DPI of the monitor
	* if _monitor is nullptr, uses the current monitor
	*/
	float getDpi(GLFWmonitor * _monitor = nullptr);

	/* Set the current cursor mode
	*  One of...
	* GLFW_CURSOR_DISABLED
	* GLFW_CURSOR_HIDDEN
	* GLFW_CURSOR_NORMAL
	*/
	void setCursorMode(int _mode);
}