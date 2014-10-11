#pragma once

#include <GLFW/glfw3.h>
#include "Dimension.h"

#include "Vox.h"

class System{
private:
	System();
	~System();
public:
	//Returns the width of the current context in pixels
	static int getScreenWidth();
	//Returns the height of the current context in pixels
	static int getScreenHeight();
	//Returns the width and height of the current context in pixels as a Dimension object
	static Dimension getScreenDimensions();
};