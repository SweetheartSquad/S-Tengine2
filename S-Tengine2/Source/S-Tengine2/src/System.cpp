#include "System.h"

unsigned long int vox::getScreenWidth(){
	return getScreenDimensions().y;
}

unsigned long int vox::getScreenHeight(){
	return getScreenDimensions().x;
}

glm::uvec2 vox::getScreenDimensions(){
	int screenHeight;
	int screenWidth;

	glfwGetWindowSize(vox::currentContext, &screenWidth, &screenHeight);

	return glm::uvec2(screenWidth, screenHeight);
}