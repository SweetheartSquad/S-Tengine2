#pragma once

#include <UILayer.h>

#include <MeshEntity.h>
#include <MatrixStack.h>
#include <RenderOptions.h>
#include <shader\ShaderComponentTexture.h>
#include <System.h>

UILayer::UILayer(float _left, float _right, float _top, float _bottom) : 
	Entity(),
	cam(-_right, -_left, _bottom, _top, -1000.f, 1000.f),
	shader(new BaseComponentShader(true))
{
	glm::vec2 sd = vox::getScreenDimensions();
	cam.left = -sd.x;
	cam.top = sd.y;
	Transform * t = new Transform();
	t->addChild(&cam);

	shader->addComponent(new ShaderComponentTexture(shader));
	shader->compileShader();
}

UILayer::~UILayer(){
}


void UILayer::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}

	Shader * prev = _renderOptions->overrideShader;
	glm::mat4 p = _matrixStack->getProjectionMatrix();
	glm::mat4 v = _matrixStack->getViewMatrix();

	_renderOptions->overrideShader = shader;

	_matrixStack->pushMatrix();
	_matrixStack->setProjectionMatrix(cam.getProjectionMatrix());
	_matrixStack->setViewMatrix(cam.getViewMatrix());
	Entity::render(_matrixStack, _renderOptions);
	_matrixStack->setViewMatrix(v);
	_matrixStack->setProjectionMatrix(p);
	_matrixStack->popMatrix();

	_renderOptions->overrideShader = prev;

	if(depth == GL_TRUE){
		glEnable(GL_DEPTH_TEST);
	}
}

void UILayer::resize(float _left, float _right, float _top, float _bottom){
	cam.left = -_right;
	cam.right = -_left;
	cam.top = _bottom;
	cam.bottom = _top;
}

/*bool UILayer::addChild(NodeChild * _child){
	bool success = NodeHierarchical::addChild(_child);
	if(success){
		MeshEntity * me = dynamic_cast<MeshEntity *>(_child);
		if(me != nullptr){
			me->setShader(shader, true);
		}
	}
	return success;
}*/