#include "Vox.h"

std::vector<glm::mat4>* vox::matrixStack = new std::vector<glm::mat4>;
glm::mat4 vox::currentModelMatrix = glm::mat4(1);

double vox::lastTimestamp = 0;
double vox::deltaTimeCorrection = 1;

GLFWwindow * vox::currentContext = nullptr;
bool vox::fullscreen = false;

void vox::setGlfwWindowHints(){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	Keyboard *keyboard = &Keyboard::getInstance();
	if(action == GLFW_PRESS){
		keyboard->keyDownListener(key);	
	}else if(action == GLFW_RELEASE){
		keyboard->keyUpListener(key);	
	}
}
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	Mouse * mouse = &Mouse::getInstance();
	if(action == GLFW_PRESS){
		mouse->mouseDownListener(button);
	}else if(action == GLFW_RELEASE){
		mouse->mouseUpListener(button);	
	}
}
static void mousePostionCallback(GLFWwindow *window, double x, double y){
	Mouse *mouse = &Mouse::getInstance();
	mouse->mousePositionListener(x, y);
}
void vox::initWindow(){
	glfwMakeContextCurrent(currentContext);
	glfwSetKeyCallback(currentContext, keyCallback);
	glfwSetMouseButtonCallback(currentContext, mouseButtonCallback);
	glfwSetCursorPosCallback(currentContext, mousePostionCallback);
}

/////////// Matrix Stack Begin //////////////

void vox::calculateModelMatrix()
{
	glm::mat4 returnMatrix = glm::mat4(1);
	for(glm::mat4 m : *matrixStack){
		returnMatrix = returnMatrix * m;
	}
	currentModelMatrix = returnMatrix;
}

void vox::pushMatrix(glm::mat4 modelMatrix)
{
	matrixStack->push_back(modelMatrix);
	calculateModelMatrix();
}

void vox::popMatrix()
{
	if(matrixStack->size() > 0){
		matrixStack->pop_back();	
		calculateModelMatrix();
	}
}

glm::mat4 vox::modelMatrix(){
	return currentModelMatrix;
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
