#include "MainScene.h"

Cube *cube;

MainScene::MainScene():Scene()
{
	cube = new Cube(gmtl::Vec3f(0.f, 0.f, 0.5f),0.2f);
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
	cube->rotate(2,0,1,0);
}

void MainScene::draw()
{
	Scene::draw();
}