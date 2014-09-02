#include "MainScene.h"

Cube *cube;
Cube *cube2;
Cube *cube3;
Cube *cube4;
MainScene::MainScene():Scene()
{
	cube = new Cube(glm::vec3(0.f, 0.f, 0.5f),0.2f);
	addChild(cube);
	cube->shader = new ShaderInterface("../assets/ColourShader");
	
	cube->vertices->pop_back();
	cube->vertices->pop_back();
	cube->vertices->pop_back();
	cube->vertices->pop_back();

	cube->vertexInterface->configureVertexAttributes(cube->shader->get_aPositionVertex(), 3, 0);
	cube->vertexInterface->configureVertexAttributes(cube->shader->get_aFragColor(), 4, sizeof(float)*3);
	cube->vertexInterface->configureVertexAttributes(cube->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	/*cube->setFrontColour(1,0,0, 1);
	cube->setLeftColour(0,1,0, 1);
	cube->setBackColour(0,0,1, 1);
	cube->setBottomColour(1,1,0, 1);
	cube->setTopColour(1,0,1, 1);*/
	//cube->setRightColour(0,1,1, 1);

	cube->transform->translateX(0.5);

	cube->vertices->at(3).y += 1.5;
	
	cube->pushQuad(2,1,5,7);
	//cube->pushQuad(4,3,8,1);

	/*cube2 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube->addChild(cube2);

	cube2->shader = new ShaderInterface("../assets/junkdata");
	cube2->vertexInterface->configureVertexAttributes(cube2->shader->get_aPositionVertex(), 0, 3);
	cube2->vertexInterface->configureVertexAttributes(cube2->shader->get_aFragColor(), 4, sizeof(float)*3);
	cube2->vertexInterface->configureVertexAttributes(cube2->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube2->setFrontColour(1,0,0,1);
	cube2->setLeftColour(0,1,0,1);
	cube2->setBackColour(0,0,1,1);
	cube2->setBottomColour(1,1,0,1);
	cube2->setTopColour(1,0,1,1);
	cube2->setRightColour(0,1,1,1);
	cube2->transform->translateX(0.5);*/
	
	/*cube3 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube2->addChild(cube3);
	cube3->shader = new ShaderInterface("../assets/ColourShader");
	
	cube3->vertices->pop_back();
	cube3->vertices->pop_back();
	cube3->vertices->pop_back();
	cube3->vertices->pop_back();

	cube3->vertexInterface->configureVertexAttributes(cube3->shader->get_aPositionVertex(), 3, 0);
	cube3->vertexInterface->configureVertexAttributes(cube3->shader->get_aFragColor(), 4, sizeof(float)*3);
	cube3->vertexInterface->configureVertexAttributes(cube3->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube3->setFrontColour(0.1,0,0,1);
	cube3->setLeftColour(0,0.1,0,1);
	cube3->setBackColour(0,0,0.1,1);
	cube3->setBottomColour(0.1,0.1,0,1);
	cube3->setTopColour(0.1,0,0.1,1);
	//cube3->setRightColour(0,0.1,0.1,1);

	cube3->vertices->at(3).x += 0.5;

	cube3->transform->translateX(0.5);*/

	
	cube4 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	addChild(cube4);
	cube4->shader = new ShaderInterface("../assets/ColourShader");
	cube4->vertexInterface->configureVertexAttributes(cube4->shader->get_aPositionVertex(), 3, 0);
	cube4->vertexInterface->configureVertexAttributes(cube4->shader->get_aFragColor(), 4, sizeof(float)*3);
	cube4->vertexInterface->configureVertexAttributes(cube4->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	/*cube4->setFrontColour(1,0,0,1);
	cube4->setLeftColour(0,1,0,1);
	cube4->setBackColour(0,0,1,1);
	cube4->setBottomColour(1,1,0,1);
	cube4->setTopColour(1,0,1,1);
	cube4->setRightColour(0,1,1,1);*/
	cube4->transform->scale(15.0, 15.0, 15.0);
}

MainScene::~MainScene(){
}

void MainScene::update(){
	Scene::update();
	
	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		// Toggle fullscreen flag.
		vox::fullscreen = !vox::fullscreen;

		//get size
		int w = 640, h = 480;
		if(vox::fullscreen){
			const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			w = mode->width;
			h = mode->height;
			w = 50;
			h = 50;
		}

		// Close the current window.
		std::cout << std::endl << glfwGetCurrentContext() << std::endl;
		
		// Renew calls to glfwOpenWindowHint.
		// (Hints get reset after the call to glfwOpenWindow.)
		vox::setGlfwWindowHints();

		// Create the new window.
		GLFWwindow* window;

		window = glfwCreateWindow(w, h, "Simple example",  /*vox::fullscreen ? glfwGetPrimaryMonitor() : */nullptr, vox::currentContext);
		
		//glfwDestroyWindow(vox::currentContext);

		vox::currentContext = window;
		if (!window){
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		vox::initWindow();

		std::cout << glfwGetCurrentContext() << std::endl;

		GLUtils::checkForError(0,__FILE__,__LINE__);
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		cube->transform->rotate(2, 0, -1, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		cube2->transform->translateX(0.02);
		cube2->transform->rotate(2, 0, -1, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		cube->transform->rotate(2 * vox::deltaTimeCorrection , 1, 0, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_W)){
		cube->transform->rotate(2, -1, 0, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_Q)){
		cube3->transform->translateX(0.02);
		cube3->transform->rotate(2, 0, -1, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_E)){
		cube->transform->translate(vox::deltaTimeCorrection * 0.001,0,0);
	}

	if(mouse->leftDown()){
		glm::quat r = glm::angleAxis(1.f, glm::vec3(0.f,0.f,1.f));
		cube->transform->rotate(r);
	}

	if(mouse->rightDown()){
		cube->transform->scale(0.9, 0.9, 0.9);
	}
}

void MainScene::draw()
{
	Scene::draw();
}