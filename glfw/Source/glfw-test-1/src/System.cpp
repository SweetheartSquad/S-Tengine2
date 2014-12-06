#include "System.h"

int vox::getScreenWidth(){
	return getScreenDimensions().height;
}

int vox::getScreenHeight(){
	return getScreenDimensions().width;
}

Dimension vox::getScreenDimensions(){
	int screenHeight;
	int screenWidth;

	glfwGetWindowSize(vox::currentContext, &screenWidth, &screenHeight);

	return Dimension(screenWidth, screenHeight);
}