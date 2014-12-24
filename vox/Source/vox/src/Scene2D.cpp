#include "Scene2D.h"
#include "OrthographicCamera.h"
#include <GLFW/glfw3.h>

Scene2D::Scene2D(Game * _game):
	Scene(_game)
{
	camera = (new OrthographicCamera(-10, 10, -10, 10, -10, 10));
}

Scene2D::~Scene2D(){
}

void Scene2D::update(){
	Scene::update();
}

void Scene2D::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	Scene::render(_matrixStack, _renderStack);
}