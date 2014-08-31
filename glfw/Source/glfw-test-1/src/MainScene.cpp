#include "MainScene.h"

Cube *cube;
Cube *cube2;
Cube *cube3;
Cube *cube4;
MainScene::MainScene():Scene()
{
	cube = new Cube(glm::vec3(0.f, 0.f, 0.5f),0.2f);
	addChild(cube);
	cube->shader = new ShaderInterface("../assets/ColourShader.vsh","../assets/ColourShader.fsh");
	cube->vertexBuffer->configureVertexAttributes(cube->shader->get_aPositionVertex(), 3, 0);
	cube->vertexBuffer->configureVertexAttributes(cube->shader->get_aFragColor(), 4, sizeof(float)*3);
	cube->vertexBuffer->configureVertexAttributes(cube->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube->setFrontColour(1,0,0, 1);
	cube->setLeftColour(0,1,0, 1);
	cube->setBackColour(0,0,1, 1);
	cube->setBottomColour(1,1,0, 1);
	cube->setTopColour(1,0,1, 1);
	cube->setRightColour(0,1,1, 1);

	cube->transform->translateX(0.5);


	cube2 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube->addChild(cube2);

	cube2->shader = new ShaderInterface("../assets/junkdata.vert","../assets/junkdata.frag");
	cube2->vertexBuffer->configureVertexAttributes(cube2->shader->get_aPositionVertex(), 0, 3);
	cube2->vertexBuffer->configureVertexAttributes(cube2->shader->get_aFragColor(), 4, sizeof(float)*3);
	cube2->vertexBuffer->configureVertexAttributes(cube2->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube2->setFrontColour(1,0,0,1);
	cube2->setLeftColour(0,1,0,1);
	cube2->setBackColour(0,0,1,1);
	cube2->setBottomColour(1,1,0,1);
	cube2->setTopColour(1,0,1,1);
	cube2->setRightColour(0,1,1,1);
	cube2->transform->translateX(0.5);
	
	cube3 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	cube2->addChild(cube3);
	cube3->shader = new ShaderInterface("../assets/ColourShader.vsh","../assets/ColourShader.fsh");
	cube3->vertexBuffer->configureVertexAttributes(cube3->shader->get_aPositionVertex(), 3, 0);
	cube3->vertexBuffer->configureVertexAttributes(cube3->shader->get_aFragColor(), 4, sizeof(float)*3);
	cube3->vertexBuffer->configureVertexAttributes(cube3->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube3->setFrontColour(1,0,0,1);
	cube3->setLeftColour(0,1,0,1);
	cube3->setBackColour(0,0,1,1);
	cube3->setBottomColour(1,1,0,1);
	cube3->setTopColour(1,0,1,1);
	cube3->setRightColour(0,1,1,1);
	cube3->transform->translateX(0.5);

	
	cube4 = new Cube(glm::vec3(0.f, 0.f, 0.5f),1);
	addChild(cube4);
	cube4->shader = new ShaderInterface("../assets/ColourShader.vsh","../assets/ColourShader.fsh");
	cube4->vertexBuffer->configureVertexAttributes(cube4->shader->get_aPositionVertex(), 3, 0);
	cube4->vertexBuffer->configureVertexAttributes(cube4->shader->get_aFragColor(), 4, sizeof(float)*3);
	cube4->vertexBuffer->configureVertexAttributes(cube4->shader->get_aVertexNormals(), 3, sizeof(float)*7);
	
	cube4->setFrontColour(1,0,0,1);
	cube4->setLeftColour(0,1,0,1);
	cube4->setBackColour(0,0,1,1);
	cube4->setBottomColour(1,1,0,1);
	cube4->setTopColour(1,0,1,1);
	cube4->setRightColour(0,1,1,1);
	cube4->transform->scale(50.0,50.0,50.0);
}

MainScene::~MainScene()
{
}

void MainScene::update()
{
	Scene::update();
	if(keyboard->keyDown(GLFW_KEY_A))
	{
		cube->transform->rotate(2, 0, -1, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_D))
	{
		cube2->transform->translateX(0.02);
		cube2->transform->rotate(2, 0, -1, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_S))
	{
		cube->transform->rotate(2 * vox::deltaTimeCorrection , 1, 0, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_W))
	{
		cube->transform->rotate(2, -1, 0, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_Q))
	{
		cube3->transform->translateX(0.02);
		cube3->transform->rotate(2, 0, -1, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_E))
	{
		cube->transform->translate(vox::deltaTimeCorrection * 0.001,0,0);
	}

	if(mouse->leftDown())
	{
		glm::quat r = glm::angleAxis(1.f, glm::vec3(0.f,0.f,1.f));
		cube->transform->rotate(r);
	}

	if(mouse->rightDown())
	{
		cube->transform->scale(0.9, 0.9, 0.9);
	}
}

void MainScene::draw()
{
	Scene::draw();
}