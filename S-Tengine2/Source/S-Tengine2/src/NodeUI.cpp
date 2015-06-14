#pragma once

#include <NodeUI.h>
#include <Scene.h>
#include <Camera.h>
#include <Mouse.h>

NodeUI::NodeUI(BulletWorld * _world, Scene * _scene) :
	MeshEntity(nullptr),
	NodeBulletBody(_world),
	mouse(&Mouse::getInstance()),
	isHovered(false),
	isDown(false),
	isActive(false),
	layoutDirty(true),
	scene(_scene),
	onDownFunction(nullptr),
	marginLeft(0.f),
	marginRight(0.f),
	marginTop(0.f),
	marginBottom(0.f),
	paddingLeft(0.f),
	paddingRight(0.f),
	paddingTop(0.f),
	paddingBottom(0.f)
{

}


void NodeUI::down(){
	isHovered = true;
	isDown = true;
	
    // do event stuff
	if(onDownFunction != nullptr){
		onDownFunction(this);
	}
}
void NodeUI::up(){
	isDown = false;
	
	// do event stuff
}
void NodeUI::in(){
	isHovered = true;
}
void NodeUI::out(){
	isHovered = false;
}


void NodeUI::update(Step * _step){
	
	float raylength = 1000;

	/*Camera * cam = scene->activeCamera;
		
	glm::vec3 campos = cam->getWorldPos();
	glm::vec3 camdir = cam->forwardVectorRotated;
	btVector3 raystart(campos.x, campos.y, campos.z);
	btVector3 raydir(camdir.x, camdir.y, camdir.z);
	btVector3 rayend = raystart + raydir*raylength;*/
	
	btVector3 raystart(mouse->mouseX(), mouse->mouseY(), -raylength);
	btVector3 raydir(0, 0, 1);
	btVector3 rayend(mouse->mouseX(), mouse->mouseY(), raylength);
	
	/*btCollisionWorld::ClosestRayResultCallback RayCallback(raystart, rayend);
	world->world->rayTest(raystart, rayend, RayCallback);
	if(RayCallback.hasHit()){

	}*/
	btTransform rayfrom;
	rayfrom.setIdentity(); rayfrom.setOrigin(raystart);
	btTransform rayto;
	rayto.setIdentity(); rayto.setOrigin(rayend);
	btCollisionWorld::AllHitsRayResultCallback raycb(raystart, rayend);
	world->world->rayTestSingle(rayfrom, rayto, body, shape, body->getWorldTransform(), raycb);
	
	
	if(raycb.hasHit()){
		if(!isHovered){
			in();
		}if(mouse->leftJustPressed()){
			down();
		}else if(mouse->leftJustReleased()){
			up();
		}
	}else{
		if(isHovered){
			out();
		}
		if(isDown && mouse->leftJustReleased()){
			isDown = false;
		}
	}

	NodeBulletBody::update(_step);
	Entity::update(_step);
}

void NodeUI::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	if(isHovered){
		if(isDown){
			renderDown(_matrixStack, _renderOptions);
		}else{
			renderOver(_matrixStack, _renderOptions);
		}
	}else if(isActive){
		renderActive(_matrixStack, _renderOptions);
	}else{
		renderDefault(_matrixStack, _renderOptions);
	}
}

void NodeUI::renderDown(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//std::cout << "down" << std::endl;
	renderDefault(_matrixStack, _renderOptions);
}

void NodeUI::renderOver(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//std::cout << "over" << std::endl;
	renderDefault(_matrixStack, _renderOptions);
}

void NodeUI::renderActive(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	renderDefault(_matrixStack, _renderOptions);
}

void NodeUI::setMarginLeft(float _margin){
	marginLeft = _margin;
}

void NodeUI::setMarginRight(float _margin){
	marginRight = _margin;
}

void NodeUI::setMarginTop(float _margin){
	marginTop = _margin;
}

void NodeUI::setMarginBottom(float _margin){
	marginBottom = _margin;
}

void NodeUI::setPaddingLeft(float _padding){
	paddingLeft = _padding;
}

void NodeUI::setPaddingRight(float _padding){
	paddingRight = _padding;
}

void NodeUI::setPaddingTop(float _padding){
	paddingTop = _padding;
}

void NodeUI::setPaddingBottom(float _padding){
	paddingBottom = _padding;
}

float NodeUI::getMarginLeft(){
	return marginLeft;
}

float NodeUI::getMarginRight(){
	return marginRight;
}

float NodeUI::getMarginTop(){
	return marginTop;
}

float NodeUI::getMarginBottom(){
	return marginBottom;
}

float NodeUI::getPaddingLeft(){
	return paddingLeft;
}

float NodeUI::getPaddingRight(){
	return paddingRight;
}

float NodeUI::getPaddingTop(){
	return paddingTop;
}

float NodeUI::getPaddingBottom(){
	return paddingBottom;
}

bool NodeUI::isLayoutDirty(){
	return layoutDirty;
}

void NodeUI::updateCollider(){
	setColliderAsBox(getMeasuredWidth() * 0.5f, getMeasuredHeight() *0.5f ,0.1);
	createRigidBody(0);
}

void NodeUI::renderDefault(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	//std::cout << "default" << std::endl;
	Entity::render(_matrixStack, _renderOptions);
}