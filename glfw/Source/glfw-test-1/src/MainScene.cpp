#include "MainScene.h"


Cube *cube;

MainScene::MainScene():Scene()
{
	cube = new Cube(glm::vec3(0.f, 0.f, 0.5f),0.2f);
	addChild(cube);
	cube->shader = new ShaderInterface("../assets/ColourShader.vsh","../assets/ColourShader.fsh");
	cube->vertexBuffer->configureVertexAttributes(cube->shader->get_aPositionVertex(), 0);
	cube->vertexBuffer->configureVertexAttributes(cube->shader->get_aFragColor(), sizeof(float)*3);
	
	cube->setFrontColour(1,0,0,1);
	cube->setLeftColour(0,1,0,1);
	cube->setBackColour(0,0,1,1);
	cube->setBottomColour(1,1,0,1);
	cube->setTopColour(1,0,1,1);
	cube->setRightColour(0,1,1,1);
	cube->translateX(0.5);
}

MainScene::~MainScene()
{

}

void MainScene::update()
{
	Scene::update();

	//if(keyboard->keyDown(GLFW_KEY_LEFT_ALT))
	//{
		camera->update();
	//}
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
		cube->translate(0.001,0,0);
	}

	if(mouse->leftDown())
	{
		glm::quat r = glm::angleAxis(1.f, glm::vec3(0.f,0.f,1.f));
		cube->rotate(r);
	}

	if(mouse->rightDown())
	{
		cube->scale(0.9, 0.9, 0.9);
	}

	//glfwSetCursorPos(glfwGetCurrentContext(), 320, 240);
}

void MainScene::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{

		Scene::draw(projectionMatrix, viewMatrix);
	
}