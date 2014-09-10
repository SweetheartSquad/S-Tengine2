#include "Vox.h"

std::vector<glm::mat4>* vox::matrixStack = new std::vector<glm::mat4>;
glm::mat4 vox::currentModelMatrix = glm::mat4(1);

double vox::lastTimestamp = 0;
double vox::deltaTimeCorrection = 1;

bool vox::fullscreen = false;

GLFWwindow * vox::currentContext = nullptr;

void vox::setGlfwWindowHints(){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

static void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods){
	Keyboard *keyboard = &Keyboard::getInstance();
	if(_action == GLFW_PRESS){
		keyboard->keyDownListener(_key);
	}else if(_action == GLFW_RELEASE){
		keyboard->keyUpListener(_key);
	}
}
static void mouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods){
	Mouse * mouse = &Mouse::getInstance();
	if(_action == GLFW_PRESS){
		mouse->mouseDownListener(_button);
	}else if(_action == GLFW_RELEASE){
		mouse->mouseUpListener(_button);
	}
}
static void mousePostionCallback(GLFWwindow *_window, double _x, double _y){
	Mouse *mouse = &Mouse::getInstance();
	mouse->mousePositionListener(_x, _y);
}
void vox::initWindow(GLFWwindow * _w){
	if(_w != nullptr){
		//glfwMakeContextCurrent(currentContext);
		glfwSetKeyCallback(_w, keyCallback);
		glfwSetMouseButtonCallback(_w, mouseButtonCallback);
		glfwSetCursorPosCallback(_w, mousePostionCallback);
	}
}

/////////// Matrix Stack Begin //////////////
void vox::popMatrix(){
	if(matrixStack->size() > 0){
		if(matrixStack->size()>0){
			currentModelMatrix = matrixStack->at(matrixStack->size()-1);
			matrixStack->pop_back();
		}else{
			currentModelMatrix = glm::mat4(1);
			throw;
		}
	}else{
		currentModelMatrix = glm::mat4(1);
		throw;
	}
}

void vox::pushMatrix(){
	matrixStack->push_back(currentModelMatrix);
}

glm::mat4 vox::getCurrentMatrix(){
	return currentModelMatrix;
}

void vox::scale(glm::mat4 _scaleMatrix){
	currentModelMatrix = currentModelMatrix * _scaleMatrix;
}

void vox::rotate(glm::mat4 _rotationMatrix){
	currentModelMatrix = currentModelMatrix * _rotationMatrix;
}

void vox::translate(glm::mat4 _translationMatrix){
	currentModelMatrix = currentModelMatrix * _translationMatrix;
}

void vox::applyMatrix(glm::mat4 _modelMatrix){
	currentModelMatrix = currentModelMatrix * _modelMatrix;
}

void vox::clearMatrixStack(){
	matrixStack->clear();
}

/////////// Delta Time Begin //////////////
void vox::calculateDeltaTimeCorrection(){
	double targetFrameDuration = static_cast<double>(1)/FPS;
	double time = glfwGetTime();
	double deltaTime = time - lastTimestamp;
	deltaTimeCorrection = deltaTime/(targetFrameDuration);
	lastTimestamp = time;
}

void vox::destruct(){
	delete matrixStack;
}