#include "Scene2D.h"
#include "OrthographicCamera.h"

#include <GLFW/glfw3.h>

Scene2D::Scene2D(Game * _game):
	Scene(_game)
{
	camera = (new OrthographicCamera(10, -10, -10, 10, -10, 10));
}

Scene2D::~Scene2D(){
}

void Scene2D::update(Step * _step){
	Scene::update(_step);
}

void Scene2D::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	Scene::render(_matrixStack, _renderOptions);
}