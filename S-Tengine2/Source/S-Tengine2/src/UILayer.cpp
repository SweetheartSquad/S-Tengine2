#pragma once

#include <UILayer.h>

#include <MatrixStack.h>
#include <RenderOptions.h>
#include <shader\ShaderComponentTexture.h>

UILayer::UILayer() : 
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	Entity(nullptr),
	cam(-1920.f, 0, 0, 1080.f, -1000.f, 1000.f),
	shader(new BaseComponentShader(true))
{
	shader->components.push_back(new ShaderComponentTexture(shader));
	shader->compileShader();
}

UILayer::~UILayer(){
	shader->decrementAndDelete();
}


void UILayer::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Shader * prev = _renderOptions->overrideShader;
	_renderOptions->overrideShader = shader;

	_matrixStack->pushMatrix();
	glm::mat4 p = _matrixStack->getProjectionMatrix();
	glm::mat4 v = _matrixStack->getViewMatrix();
	_matrixStack->setProjectionMatrix(cam.getProjectionMatrix());
	_matrixStack->setViewMatrix(cam.getViewMatrix());
	Entity::render(_matrixStack, _renderOptions);
	_matrixStack->popMatrix();
	_matrixStack->setViewMatrix(v);
	_matrixStack->setProjectionMatrix(p);

	
	_renderOptions->overrideShader = prev;
}