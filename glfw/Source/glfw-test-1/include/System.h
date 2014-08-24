#pragma once 

#include <GLFW/glfw3.h>
#include "Dimension.h"

class System
{
private:
	System();
	~System();
public:
	static int getScreenWidth();
	static int getScreenHeight();
	static Dimension getScreenDimensions();
};