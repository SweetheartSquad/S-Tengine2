#include "System.h"

unsigned long int sweet::getScreenWidth(){
	return getScreenDimensions().y;
}

unsigned long int sweet::getScreenHeight(){
	return getScreenDimensions().x;
}

glm::uvec2 sweet::getScreenDimensions(){
	int screenHeight;
	int screenWidth;

	glfwGetWindowSize(sweet::currentContext, &screenWidth, &screenHeight);

	return glm::uvec2(screenWidth, screenHeight);
}