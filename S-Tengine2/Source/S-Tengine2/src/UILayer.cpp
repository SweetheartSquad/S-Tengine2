#pragma once

#include <UILayer.h>

#include <MeshEntity.h>
#include <MatrixStack.h>
#include <RenderOptions.h>
#include <shader\ShaderComponentTexture.h>
#include <System.h>

UILayer::UILayer(float _left, float _right, float _bottom, float _top) : 
	Entity(),
	cam(_left, _right, _bottom, _top, -1000.f, 1000.f),
	shader(new ComponentShaderBase(true)),
	bulletWorld(new BulletWorld()),
	bulletDebugDrawer(new BulletDebugDrawer(bulletWorld->world))
{
	Transform * t = new Transform();
	t->addChild(&cam);

	shader->addComponent(new ShaderComponentTexture(shader));
	shader->compileShader();

	bulletDebugDrawer->setDebugMode(btIDebugDraw::DBG_NoDebug);
	bulletWorld->world->setDebugDrawer(bulletDebugDrawer);
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
}

void UILayer::update(Step * _step){
	Entity::update(_step);

	// make sure the debug drawer is always the last thing so it will render on top
	if(bulletDebugDrawer != childTransform->children.back()){
		childTransform->addChild(bulletDebugDrawer, false);
	}
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