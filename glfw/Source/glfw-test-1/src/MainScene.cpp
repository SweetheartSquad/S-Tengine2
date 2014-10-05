#include "MainScene.h"
#include <Resource.h>

Cube * cube;
Cube * cube2;
Cube * cube3;
Cube * cube4;

Light glight;
Texture* tex;
Shader* texShader;

MainScene::MainScene():
	Scene()
{
	cube = new Cube(glm::vec3(0.f, 0.f, 0.5f),0.2f);
	cube->setShader(new Shader("../assets/diffuse", true), true);

	cube->mesh->vertices.pop_back();
	cube->mesh->vertices.pop_back();
	cube->mesh->vertices.pop_back();
	cube->mesh->vertices.pop_back();

	cube->setFrontColour(1,0,0, 1);
	cube->setLeftColour(0,1,0, 1);
	cube->setBackColour(0,0,1, 1);
	cube->setBottomColour(1,1,0, 1);
	cube->setTopColour(1,0,1, 1);

	cube->transform->translateX(0.5);
	cube->mesh->vertices.at(3).y += 1.5;
	cube->mesh->vertices.at(0).y += 1.5;
	static_cast<QuadMesh *>(cube->mesh)->pushQuad(2,1,5,7);

	texShader = new Shader("../assets/ColourShader", true);
	tex = new Texture("../assets/img_cheryl.jpg", 256, 256, true, true);
	cube->mesh->pushTexture2D(tex);

	Transform *t = new Transform();
	t->translateX(-0.5);
	t->scale(2, 2, 2);

	addChild(new Entity(Resource::loadMeshFromObj("../assets/cube.vox"),
		t, texShader, cube));

	cube2 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube2->setShader(texShader, true);

	cube2->setFrontColour(1,0,0,1);
	cube2->setLeftColour(0,1,0,1);
	cube2->setBackColour(0,0,1,1);
	cube2->setBottomColour(1,1,0,1);
	cube2->setTopColour(1,0,1,1);
	cube2->setRightColour(0,1,1,1);

	cube2->transform->translateX(0.5);
	cube2->mesh->pushTexture2D(tex);

	cube3 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube->addChild(cube2);
	cube2->addChild(cube3);
	cube3->setShader(texShader, true);

	cube3->setFrontColour(0.5,0,0,1);
	cube3->setLeftColour(0,0.5,0,1);
	cube3->setBackColour(0,0,0.5,1);
	cube3->setBottomColour(0.5,0.5,0,1);
	cube3->setTopColour(0.5,0,0.5,1);
	cube3->setRightColour(0,0.5,0.5,1);

	cube3->mesh->vertices.at(3).x += 0.5;
	cube3->transform->translateX(0.5);
	cube3->mesh->pushTexture2D(tex);

	cube4 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	addChild(cube);
	addChild(cube4);
	cube4->setShader(texShader, true);

	cube4->transform->scale(15.0, 15.0, 15.0);
	cube4->mesh->pushTexture2D(tex);

	cube->mesh->dirty = true;
	cube2->mesh->dirty = true;
	cube3->mesh->dirty = true;

	//delete cube;
	//delete cube2;
	//delete cube3;

	cube4->mesh->dirty = true;
	//delete cube4;
}

MainScene::~MainScene(){
}

void MainScene::update(){
	Scene::update();

	if(keyboard->keyJustUp(GLFW_KEY_F11)){
		toggleFullScreen();
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

void MainScene::draw(){
	Scene::draw();
}