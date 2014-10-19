#include "MainScene.h"
#include <Resource.h>

Cube * cube;
Cube * cube2;
Cube * cube3;
Cube * cube4;

Light glight;

Texture* tex;
Texture* voxTex;

Shader* texShader;
Shader* voxShader;

Light *tLight;

MainScene::MainScene(Game * _game):
	Scene(game)
{
	cube = new Cube(glm::vec3(0.f, 0.f, 0.5f),0.2f);
	cube->setShader(new Shader("../assets/diffuse", false, true), true);
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

	texShader = new Shader("../assets/diffuse", false, true);

	voxShader = new Shader("../assets/voxel", true, true);

	tex = new Texture("../assets/uv-test.jpg", 1000, 1000, true, true);
	voxTex = new Texture("../assets/voxel-texture.png", 512, 512, true, true);

	Transform *t = new Transform();
	t->translateX(-2);
	t->scale(3, 3, 3);

	for(unsigned long int i = 0; i < 3; ++i){
		Entity * loaded = new Entity(Resource::loadMeshFromObj("../assets/cube.vox"), t, voxShader, cube);
		loaded->mesh->polygonalDrawMode = GL_POINTS;
		cube->addChild(loaded);
		//loaded->mesh->pushTexture2D(tex);
	}

	//Entity * loaded1 = new Entity(Resource::loadMeshFromObj("../assets/cube.vox"), t, texShader, cube);

	//cube->addChild(loaded1);

	cube2 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube2->setShader(texShader, true);
	cube2->transform->translateX(0.5);
	cube2->mesh->pushTexture2D(tex);

	cube3 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube->addChild(cube2);
	//cube2->addChild(cube3);
	addChild(cube3);
	cube3->setShader(texShader, true);

	cube3->mesh->vertices.at(3).x += 0.5;
	cube3->transform->translateX(0.5);
	cube3->mesh->pushTexture2D(tex);

	cube4 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	addChild(cube);
	addChild(cube4);
	cube4->setShader(texShader, true);

	cube4->transform->scale(15.0, 1.0, 15.0);
	cube4->transform->translateY(-2);
	cube4->mesh->pushTexture2D(tex);

	cube->mesh->dirty = true;
	cube2->mesh->dirty = true;
	cube3->mesh->dirty = true;
	cube4->mesh->dirty = true;

	tLight = new Light();
	tLight->data.position = glm::vec3(-3.f, 1.5f, 1.f);
	tLight->data.intensities = glm::vec3(0.5f, 0.7f, 0.5f);

	Light *tLight2 = new Light();
	tLight2->data.position = glm::vec3(1.f, -1.5, 1.f);
	tLight2->data.intensities = glm::vec3(1.7f, 1.5f, 1.5f);

	lights.push_back(tLight);
	lights.push_back(tLight2);
}

MainScene::~MainScene(){
}

void MainScene::update(){
	Scene::update();

	*cube3->transform = *tLight->transform;

	tLight->transform->translateX(sin(glfwGetTime()) * 0.1);
	tLight->transform->translateZ(cos(glfwGetTime()) * 0.1);

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

void MainScene::render(){
	Scene::render();
}