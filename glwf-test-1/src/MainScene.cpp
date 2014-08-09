#include "MainScene.h"

Cube *cube;

MainScene::MainScene():Scene()
{
	cube = new Cube(glm::vec3(0.f, 0.f, 0.5f),0.2f);
	addChild(cube);
	cube->shader = new ShaderInterface("../assets/ColourShader.vsh","../assets/ColourShader.fsh");
	cube->vertexBuffer->configureVertexAttributes(cube->shader->get_aPositionVertex(), 0);
	cube->vertexBuffer->configureVertexAttributes(cube->shader->get_aFragColor(),sizeof(float)*3);
}

MainScene::~MainScene()
{

}

void MainScene::update()
{
	Scene::update();

	if(keyboard->keyDown(GLFW_KEY_A))
	{
		cube->rotate(2, 0, -1, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_D))
	{
		cube->rotate(2, 0, 1, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_S))
	{
		cube->rotate(2, 1, 0, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_W))
	{
		cube->rotate(2, -1, 0, 0);
	}
	if(keyboard->keyDown(GLFW_KEY_Q))
	{
		cube->rotate(2, 0, 0, -1);
	}
	if(keyboard->keyDown(GLFW_KEY_E))
	{
		cube->rotate(2, 0, 0, 1);
	}

	if(mouse->leftDown())
	{
		cube->scale(1.1, 1.1, 1.1);
	}

	if(mouse->rightDown())
	{
		cube->scale(0.9, 0.9, 0.9);
	}
}

void MainScene::draw()
{
	Scene::draw();
}