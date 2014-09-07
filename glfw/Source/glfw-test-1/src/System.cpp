#include "System.h"

System::System(){
}

System::~System(){
}

int System::getScreenWidth(){
	return getScreenDimensions().height;
}

int System::getScreenHeight(){
	return getScreenDimensions().width;
}

Dimension System::getScreenDimensions(){
	int screenHeight;
	int screenWidth;

	glfwGetWindowSize(vox::currentContext, &screenWidth, &screenHeight);

	return Dimension(screenWidth, screenHeight);
}