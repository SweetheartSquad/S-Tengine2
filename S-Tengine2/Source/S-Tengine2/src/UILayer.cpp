#pragma once

#include <UILayer.h>

#include <MeshEntity.h>
#include <MatrixStack.h>
#include <RenderOptions.h>
#include <shader\ShaderComponentTexture.h>
#include <System.h>

UILayer::UILayer(Scene * _scene, float _left, float _right, float _bottom, float _top) : 
	NodeUI(world, _scene),
	NodeBulletBody(new BulletWorld()),
	cam(_left, _right, _bottom, _top, -1000.f, 1000.f),
	shader(new ComponentShaderBase(true)),
	bulletDebugDrawer(new BulletDebugDrawer(world->world))
{
	Transform * t = new Transform();
	t->addChild(&cam);

	shader->addComponent(new ShaderComponentTexture(shader));
	shader->compileShader();

	bulletDebugDrawer->setDebugMode(btIDebugDraw::DBG_NoDebug);
	world->world->setDebugDrawer(bulletDebugDrawer);
	childTransform->addChild(bulletDebugDrawer, false);
}

UILayer::~UILayer(){
}


void UILayer::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}

	Shader * prev = _renderOptions->overrideShader;
	const glm::mat4 * p = _matrixStack->getProjectionMatrix();
	const glm::mat4 * v = _matrixStack->getViewMatrix();

	//_renderOptions->overrideShader = shader;

	_matrixStack->pushMatrix();
	_matrixStack->setProjectionMatrix(&cam.getProjectionMatrix());
	_matrixStack->setViewMatrix(&cam.getViewMatrix());
	Entity::render(_matrixStack, _renderOptions);
	_matrixStack->setViewMatrix(v);
	_matrixStack->setProjectionMatrix(p);
	_matrixStack->popMatrix();

	_renderOptions->overrideShader = prev;

	

	if(depth == GL_TRUE){
		glEnable(GL_DEPTH_TEST);
	}
}

void UILayer::resize(float _left, float _right, float _bottom, float _top){
	cam.left = _left;
	cam.right = _right;
	cam.bottom = _bottom;
	cam.top = _top;
	
	setWidth(_right - _left);
	setHeight(_top - _bottom);
}

void UILayer::update(Step * _step){
	Entity::update(_step);

	// make sure the debug drawer is always the last thing so it will render on top
	if(bulletDebugDrawer != childTransform->children.back()){
		childTransform->addChild(bulletDebugDrawer, false);
	}
}

float UILayer::getWidth(){
	return cam.right - cam.left;
}
float UILayer::getHeight(){
	return cam.top - cam.bottom;
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