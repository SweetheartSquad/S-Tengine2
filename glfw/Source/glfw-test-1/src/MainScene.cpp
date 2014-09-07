#include "MainScene.h"

Cube * cube;
Cube * cube2;
Cube * cube3;
Cube * cube4;
MainScene::MainScene():
	Scene()
{
	cube = new Cube(glm::vec3(0.f, 0.f, 0.5f),0.2f);
	cube->shader = new ShaderInterface("../assets/ColourShader");

	cube->mesh->configureVertexAttributes(cube->shader->get_aVertexPosition(), 3, 0);
	cube->mesh->configureVertexAttributes(cube->shader->get_aVertexColor(), 4, sizeof(float)*3);
	cube->mesh->configureVertexAttributes(cube->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube->mesh->vertices.pop_back();
	cube->mesh->vertices.pop_back();
	cube->mesh->vertices.pop_back();
	cube->mesh->vertices.pop_back();
	
	cube->setFrontColour(1,0,0, 1);
	cube->setLeftColour(0,1,0, 1);
	cube->setBackColour(0,0,1, 1);
	cube->setBottomColour(1,1,0, 1);
	cube->setTopColour(1,0,1, 1);
	//cube->setRightColour(0,1,1, 1);

	cube->transform->translateX(0.5);
	cube->mesh->vertices.at(3).y += 1.5;
	cube->mesh->vertices.at(0).y += 1.5;
	((QuadMesh *)cube->mesh)->pushQuad(2,1,5,7);

	cube2 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube2->shader = new ShaderInterface("../assets/junkdata");
	cube2->mesh->configureVertexAttributes(cube2->shader->get_aVertexPosition(), 3, 0);
	cube2->mesh->configureVertexAttributes(cube2->shader->get_aVertexColor(), 4, sizeof(float)*3);
	cube2->mesh->configureVertexAttributes(cube2->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube2->setFrontColour(1,0,0,1);
	cube2->setLeftColour(0,1,0,1);
	cube2->setBackColour(0,0,1,1);
	cube2->setBottomColour(1,1,0,1);
	cube2->setTopColour(1,0,1,1);
	cube2->setRightColour(0,1,1,1);

	cube2->transform->translateX(0.5);
	
	cube3 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube->addChild(cube2);
	cube2->addChild(cube3);
	cube3->shader = new ShaderInterface("../assets/ColourShader");
	cube3->mesh->configureVertexAttributes(cube3->shader->get_aVertexPosition(), 3, 0);
	cube3->mesh->configureVertexAttributes(cube3->shader->get_aVertexColor(), 4, sizeof(float)*3);
	cube3->mesh->configureVertexAttributes(cube3->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube3->setFrontColour(0.5,0,0,1);
	cube3->setLeftColour(0,0.5,0,1);
	cube3->setBackColour(0,0,0.5,1);
	cube3->setBottomColour(0.5,0.5,0,1);
	cube3->setTopColour(0.5,0,0.5,1);
	cube3->setRightColour(0,0.5,0.5,1);

	cube3->mesh->vertices.at(3).x += 0.5;
	cube3->transform->translateX(0.5);


	
	cube4 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	addChild(cube);
	addChild(cube4);
	cube4->shader = new ShaderInterface("../assets/ColourShader");
	cube4->mesh->configureVertexAttributes(cube4->shader->get_aVertexPosition(), 3, 0);
	cube4->mesh->configureVertexAttributes(cube4->shader->get_aVertexColor(), 4, sizeof(float)*3);
	cube4->mesh->configureVertexAttributes(cube4->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	/*cube4->setFrontColour(1,0,0,1);
	cube4->setLeftColour(0,1,0,1);
	cube4->setBackColour(0,0,1,1);
	cube4->setBottomColour(1,1,0,1);
	cube4->setTopColour(1,0,1,1);
	cube4->setRightColour(0,1,1,1);*/
	cube4->transform->scale(15.0, 15.0, 15.0);

	
	cube->mesh->dirty = true;
	cube2->mesh->dirty = true;
	cube3->mesh->dirty = true;
	cube4->mesh->dirty = true;
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
		}

		std::cout << std::endl << glfwGetCurrentContext() << std::endl;
		
		// Renew calls to glfwOpenWindowHint.
		//vox::setGlfwWindowHints();

		// Create the new window.
		GLFWwindow* window;
		window = glfwCreateWindow(w, h, "Simple example 2",  vox::fullscreen ? glfwGetPrimaryMonitor() : nullptr, /*glfwGetCurrentContext()*/nullptr);
		if(!window){
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		vox::initWindow(window);

		vox::contexts.pop_back();
		glfwDestroyWindow(glfwGetCurrentContext());

		vox::contexts.push_back(window);
		glfwMakeContextCurrent(window);

		for(Entity * e : *children){
			e->unload();
		}for(Entity * e : *children){
			e->reset();
		}

		std::cout << glfwGetCurrentContext() << std::endl;

		GLUtils::checkForError(0,__FILE__,__LINE__);
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		cube->transform->rotate(2.f, 0.f, -1.f, 0.f);
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		cube2->transform->translateX(0.02f);
		cube2->transform->rotate(2.f, 0.f, -1.f, 0.f);
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		cube->transform->rotate(2.f * (float)vox::deltaTimeCorrection , 1.f, 0.f, 0.f);
	}
	if(keyboard->keyDown(GLFW_KEY_W)){
		cube->transform->rotate(2.f, -1.f, 0.f, 0.f);
	}
	if(keyboard->keyDown(GLFW_KEY_Q)){
		cube3->transform->translateX(0.02f);
		cube3->transform->rotate(2.f, 0.f, -1.f, 0.f);
	}
	if(keyboard->keyDown(GLFW_KEY_E)){
		cube->transform->translate((float)vox::deltaTimeCorrection * 0.001f, 0.f, 0.f);
	}

	if(mouse->leftDown()){
		glm::quat r = glm::angleAxis(1.f, glm::vec3(0.f, 0.f, 1.f));
		cube->transform->rotate(r);
	}

	if(mouse->rightDown()){
		cube->transform->scale(0.9f, 0.9f, 0.9f);
	}

	//camera controls
	if (keyboard->keyDown(GLFW_KEY_UP)){
		camera->transform->translate((camera->forwardVectorRotated) * camera->speed);
	}
	// Move backward
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		camera->transform->translate((camera->forwardVectorRotated) * -camera->speed);
	}
	// Strafe right
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		camera->transform->translate((camera->rightVectorRotated) * camera->speed);
	}
	// Strafe left
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		camera->transform->translate((camera->rightVectorRotated) * -camera->speed);
	}
}

void MainScene::draw()
{
	Scene::draw();
}