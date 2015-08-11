#pragma once

#include <UILayer.h>

#include <MeshEntity.h>
#include <MatrixStack.h>
#include <RenderOptions.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentMVP.h>
#include <System.h>
#include <Mouse.h>
#include <Scene.h>

RayTestInfo::RayTestInfo() :
	raycb(raystart, rayend)
{
}

UILayer::UILayer(Scene * _scene, float _left, float _right, float _bottom, float _top) : 
	NodeUI(new BulletWorld(), _scene, true),
	mouse(&Mouse::getInstance()),
	cam(_left, _right, _bottom, _top, -1000.f, 1000.f),
	bulletDebugDrawer(new BulletDebugDrawer(world->world)),
	shader(new ComponentShaderBase(true))
{
	shader->addComponent(new ShaderComponentTexture(shader));
	shader->addComponent(new ShaderComponentMVP(shader));
	shader->compileShader();
	++shader->referenceCount;

	Transform * t = new Transform();
	t->addChild(&cam);

	bulletDebugDrawer->setDebugMode(btIDebugDraw::DBG_NoDebug);
	world->world->setDebugDrawer(bulletDebugDrawer);
	childTransform->addChild(bulletDebugDrawer, false);
}

UILayer::~UILayer(){
	shader->decrementAndDelete();
}

void UILayer::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}

	Shader * prev = _renderOptions->overrideShader;
	const glm::mat4 * p = _matrixStack->getProjectionMatrix();
	const glm::mat4 * v = _matrixStack->getViewMatrix();

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

	if(mouseEnabled){
		float raylength = 1000;

		//Camera * cam = scene->activeCamera;
		//glm::vec3 campos = cam->getWorldPos();
		//glm::vec3 camdir = cam->forwardVectorRotated;
		//btVector3 raystart(campos.x, campos.y, campos.z);
		//btVector3 raydir(camdir.x, camdir.y, camdir.z);
		//btVector3 rayend = raystart + raydir*raylength;
	
		rayInfo.raystart = btVector3(mouse->mouseX(), mouse->mouseY(), -raylength);
		rayInfo.raydir = btVector3(0, 0, 1);
		rayInfo.rayend = btVector3(mouse->mouseX(), mouse->mouseY(), raylength);
	
		rayInfo.rayfrom.setIdentity(); rayInfo.rayfrom.setOrigin(rayInfo.raystart);
		rayInfo.rayto.setIdentity(); rayInfo.rayto.setOrigin(rayInfo.rayend);
		rayInfo.raycb = btCollisionWorld::AllHitsRayResultCallback(rayInfo.raystart, rayInfo.rayend);
		hitTest(this);
	}
}

void UILayer::hitTest(NodeChild * _c){
	NodeUI * ui = dynamic_cast<NodeUI *>(_c);
	if(ui != nullptr && ui->isMouseEnabled() && ui->shape != nullptr){
		rayInfo.raycb = btCollisionWorld::AllHitsRayResultCallback(rayInfo.raystart, rayInfo.rayend);
		world->world->rayTestSingle(rayInfo.rayfrom, rayInfo.rayto, ui->body, ui->shape, ui->body->getWorldTransform(), rayInfo.raycb);
		ui->setUpdateState(rayInfo.raycb.hasHit());
	}
	Entity * e = dynamic_cast<Entity *>(_c);
	if(e != nullptr){
		hitTest(e->childTransform);
		return;
	}
	Transform * t = dynamic_cast<Transform *>(_c);
	if(t != nullptr){
		for(NodeChild * c : t->children){
			hitTest(c);
		}
		return;
	}
}