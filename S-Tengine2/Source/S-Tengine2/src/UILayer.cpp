#pragma once

#include <UILayer.h>

#include <MeshEntity.h>
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
	while(children.size() > 0){
		NodeHierarchical * nh = dynamic_cast<NodeHierarchical *>(children.back());
		if(nh != nullptr){
			NodeHierarchical::deleteRecursively(nh);
		}else{
			delete children.back();	
		}children.pop_back();
	}
}


void UILayer::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
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
}

bool UILayer::addChild(NodeChild * _child){
	bool success = NodeHierarchical::addChild(_child);
	if(success){
		MeshEntity * me = dynamic_cast<MeshEntity *>(_child);
		if(me != nullptr){
			me->setShader(shader, true);
		}
	}
	return success;
}