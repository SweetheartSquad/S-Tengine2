#pragma once

#include <MeshFactory.h>
#include <NodeUI.h>
#include <Scene.h>
#include <Camera.h>
#include <Mouse.h>
#include <MeshInterface.h>

#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentAlpha.h>
#include <shader\ShaderComponentTint.h>

#include <NumberUtils.h>

NodeUI::NodeUI(BulletWorld * _world, Scene * _scene) :
	NodeBulletBody(_world),
	Entity(),
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
	paddingBottom(0.f),
	background(new MeshEntity(MeshFactory::getPlaneMesh())),
	contents(new Transform()),
	horizontalAlignment(kLEFT),
	verticalAlignment(kBOTTOM),
	autoResizingWidth(false),
	autoResizingHeight(false),
	width(0.f),
	height(0.f)
{
	ComponentShaderBase * shader = new ComponentShaderBase(true);
	shader->addComponent(new ShaderComponentTexture(shader));
	shader->addComponent(new ShaderComponentTint(shader));
	shader->addComponent(new ShaderComponentAlpha(shader));
	shader->compileShader();
	background->setShader(shader, true);
	for(unsigned long int i = 0; i < background->mesh->vertices.size(); ++i){
		background->mesh->vertices.at(i).x += 0.5f;
		background->mesh->vertices.at(i).y += 0.5f;
	}
	background->mesh->dirty = true;

	childTransform->addChild(background, true);
	childTransform->addChild(contents, false);

	updateCollider();

	setBackgroundColour(vox::NumberUtils::randomFloat(-1, 0), vox::NumberUtils::randomFloat(-1, 0), vox::NumberUtils::randomFloat(-1, 0));
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


Transform * NodeUI::addChild(NodeUI* _uiElement){
	layoutDirty = true;
	return contents->addChild(_uiElement);
}


void NodeUI::update(Step * _step){
	autoResize();

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
	//world->world->rayTestSingle(rayfrom, rayto, body, shape, body->getWorldTransform(), raycb);
	
	
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
	Entity::render(_matrixStack, _renderOptions);
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

void NodeUI::setWidth(float _width){
	width = _width;
}

void NodeUI::setHeight(float _height){
	height = _height;
}

void NodeUI::setBackgroundColour(float _r, float _g, float _b, float _a){
	dynamic_cast<ShaderComponentTint *>(dynamic_cast<ComponentShaderBase *>(background->shader)->getComponentAt(1))->setRGB(_r, _g, _b);
	dynamic_cast<ShaderComponentAlpha *>(dynamic_cast<ComponentShaderBase *>(background->shader)->getComponentAt(2))->setAlpha(_a);
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

float NodeUI::getWidth(bool _includePadding, bool _includeMargin){
	float res = width;
	if(_includePadding){
		res += getPaddingLeft() + getPaddingRight();
	}
	if(_includeMargin){
		res += getMarginLeft() + getMarginRight();
	}
	return res;
}

float NodeUI::getHeight(bool _includePadding, bool _includeMargin){
	float res = height;
	if(_includePadding){
		res += getPaddingBottom() + getPaddingTop();
	}
	if(_includeMargin){
		res += getMarginBottom() + getMarginTop();
	}
	return res;
}

bool NodeUI::isLayoutDirty(){
	return layoutDirty;
}

void NodeUI::updateCollider(){
	if(body != nullptr){
		world->world->removeRigidBody(body);
		delete body;
		body = nullptr;
	}
	if(shape != nullptr){
		delete shape;
		shape = nullptr;
	}
	setColliderAsBox(getWidth(true, false) * 0.5f, getHeight(true, false) * 0.5f, 1.f);
	createRigidBody(0);
}

void NodeUI::autoResize(){
	if(autoResizingWidth){
		autoResizeWidth();
	}
	if(autoResizingHeight){
		autoResizeHeight();
	}
	// Adjust the size of the background
	background->parents.at(0)->scale(getWidth(true, false), getHeight(true, false), 1.0f, false);
	repositionChildren();
}
void NodeUI::autoResizeWidth(){
	width = 0.0f;
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				width = std::max(width, node->getWidth(true, true));
			}
		}
	}
}
void NodeUI::autoResizeHeight(){
	height = 0.0f;
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				height = std::max(height, node->getHeight(true, true));
			}
		}
	}
}

void NodeUI::repositionChildren(){
	childTransform->translate(getMarginLeft(), getMarginBottom(), 0, false);
	contents->translate(getPaddingLeft(), getPaddingBottom(), 0, false);
}