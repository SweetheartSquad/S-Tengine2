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
	verticalAlignment(kBOTTOM)
{
	measuredWidth = 0.0f;
	measuredHeight = 0.0f;

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
	
	setPadding(5);
	setMargin(5);

	setBackgroundColour(vox::NumberUtils::randomFloat(-1, 0), vox::NumberUtils::randomFloat(-1, 0), vox::NumberUtils::randomFloat(-1, 0));
	updateCollider();
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
	if(_uiElement->width.sizeMode == kRATIO){
		_uiElement->setRationalWidth(_uiElement->width.rationalSize, this);
	}if(_uiElement->height.sizeMode == kRATIO){
		_uiElement->setRationalHeight(_uiElement->height.rationalSize, this);
	}
	return contents->addChild(_uiElement);
}

signed long int NodeUI::removeChild(NodeUI* _uiElement){
	Transform * t = _uiElement->parents.at(0);
	signed long int res = contents->removeChild(t);
	t->removeChild(_uiElement);
	delete t;
	
	if(res >= 0){
		layoutDirty = true;
	}
	return res;
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

	
	
	// for testing
	float g = dynamic_cast<ShaderComponentTint *>(dynamic_cast<ComponentShaderBase *>(background->shader)->getComponentAt(1))->getGreen();
	float b = dynamic_cast<ShaderComponentTint *>(dynamic_cast<ComponentShaderBase *>(background->shader)->getComponentAt(1))->getBlue();
	if(isHovered){
		if(isDown){
			setBackgroundColour(-1.f, g, b);
		}else{
			setBackgroundColour(-0.5f, g, b);
		}
	}else{
		setBackgroundColour(0.f, g, b);
	}
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

void NodeUI::setMargin(float _all){
	setMarginLeft(_all);
	setMarginRight(_all);
	setMarginBottom(_all);
	setMarginTop(_all);
}
void NodeUI::setMargin(float _leftAndRight, float _bottomAndTop){
	setMarginLeft(_leftAndRight);
	setMarginRight(_leftAndRight);
	setMarginBottom(_bottomAndTop);
	setMarginTop(_bottomAndTop);
}
void NodeUI::setMargin(float _left, float _right, float _bottom, float _top){
	setMarginLeft(_left);
	setMarginRight(_right);
	setMarginBottom(_bottom);
	setMarginTop(_top);
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

void NodeUI::setPadding(float _all){
	setPaddingLeft(_all);
	setPaddingRight(_all);
	setPaddingBottom(_all);
	setPaddingTop(_all);
}
void NodeUI::setPadding(float _leftAndRight, float _bottomAndTop){
	setPaddingLeft(_leftAndRight);
	setPaddingRight(_leftAndRight);
	setPaddingBottom(_bottomAndTop);
	setPaddingTop(_bottomAndTop);
}
void NodeUI::setPadding(float _left, float _right, float _bottom, float _top){
	setPaddingLeft(_left);
	setPaddingRight(_right);
	setPaddingBottom(_bottom);
	setPaddingTop(_top);
}

void NodeUI::setWidth(float _width){
	if(_width < 0){
		setAutoresizeWidth();
	}else if(_width <= 1.00001f){
		setRationalWidth(_width);
	}else{
		setPixelWidth(_width);
	}
}

void NodeUI::setHeight(float _height){
	if(_height < 0){
		setAutoresizeHeight();
	}else if(_height <= 1.00001f){
		setRationalHeight(_height);
	}else{
		setPixelHeight(_height);
	}
}

void NodeUI::setAutoresizeWidth(){
	width.setAutoSize();
	autoResizeWidth();
	resizeChildrenWidth();
}
void NodeUI::setAutoresizeHeight(){
	height.setAutoSize();
	autoResizeHeight();
	resizeChildrenHeight();
}

void NodeUI::setRationalWidth(float _rationalWidth, NodeUI * _parent){
	width.setRationalSize(_rationalWidth);
	if(_parent != nullptr){
		measuredWidth = _parent->getWidth() * width.rationalSize;
	}
	resizeChildrenWidth();
}

void NodeUI::setRationalHeight(float _rationalHeight, NodeUI * _parent){
	height.setRationalSize(_rationalHeight);
	if(_parent != nullptr){
		measuredHeight = _parent->getHeight() * height.rationalSize;
	}
	resizeChildrenHeight();
}

void NodeUI::setPixelWidth(float _pixelWidth){
	width.setPixelSize(_pixelWidth);
	resizeChildrenWidth();
}
void NodeUI::setPixelHeight(float _pixelHeight){
	height.setPixelSize(_pixelHeight);
	resizeChildrenHeight();
}

void NodeUI::resizeChildrenWidth(){
	// check for rational-width children and resize them
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * nui = dynamic_cast<NodeUI *>(trans->children.at(0));
				if(nui != nullptr){
					if(nui->width.sizeMode == kRATIO){
						nui->setRationalWidth(nui->width.rationalSize, this);
					}
				}
			}
		}
	}
}

void NodeUI::resizeChildrenHeight(){
	// check for rational-height children and resize them
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * nui = dynamic_cast<NodeUI *>(trans->children.at(0));
				if(nui != nullptr){
					if(nui->height.sizeMode == kRATIO){
						nui->setRationalHeight(nui->height.rationalSize, this);
					}
				}
			}
		}
	}
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

float NodeUI::getWidth(){
	switch (width.sizeMode){
	case kPIXEL:
		return width.pixelSize;
		break;
	case kRATIO:
	case kAUTO:
	default:
		return measuredWidth;
		break;
	}
}

float NodeUI::getHeight(){
	switch (height.sizeMode){
	case kPIXEL:
		return height.pixelSize;
		break;
	case kRATIO:
	case kAUTO:
	default:
		return measuredHeight;
		break;
	}
}

float NodeUI::getWidth(bool _includePadding, bool _includeMargin){
	float res = getWidth();
	if(_includePadding){
		res += getPaddingLeft() + getPaddingRight();
	}
	if(_includeMargin){
		res += getMarginLeft() + getMarginRight();
	}
	return res;
}

float NodeUI::getHeight(bool _includePadding, bool _includeMargin){
	float res = getHeight();
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
	glm::vec3 v = background->getWorldPos();
	TriMesh * m = new TriMesh();

	m->pushVert(Vertex(v.x, v.y + getHeight(true, false), 0.f));
	m->pushVert(Vertex(v.x + getWidth(true, false), v.y + getHeight(true, false), 0.f));
	m->pushVert(Vertex(v.x + getWidth(true, false), v.y, 0.f));
	m->pushVert(Vertex(v.x + getWidth(true, false), v.y, 0.f));
	m->pushVert(Vertex(v.x, v.y, 0.f));
	m->pushVert(Vertex(v.x, v.y + getHeight(true, false), 0.f));
	setColliderAsMesh(m, true);
	createRigidBody(0);
	// cleanup
	delete m;
	m = nullptr;
}

void NodeUI::autoResize(){
	if(width.sizeMode == kAUTO){
		autoResizeWidth();
	}
	if(height.sizeMode == kAUTO){
		autoResizeHeight();
	}
	// Adjust the size of the background
	background->parents.at(0)->scale(getWidth(true, false), getHeight(true, false), 1.0f, false);
	repositionChildren();
	//if(widthMode == kAUTO || heightMode == kAUTO || widthMode == kRATIO || heightMode == kRATIO){
		updateCollider();
	//}
}
void NodeUI::autoResizeWidth(){
	float w = 0.0f;
	// take the maximum of the width of the contents
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				w = std::max(w, node->getWidth(true, true));
			}
		}
	}
	measuredWidth = w;
}
void NodeUI::autoResizeHeight(){
	float h = 0.0f;
	// take the maximum of the height of the contents
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * node = dynamic_cast<NodeUI *>(trans->children.at(0));
				h = std::max(h, node->getHeight(true, true));
			}
		}
	}
	measuredHeight = h;
}

void NodeUI::repositionChildren(){
	background->parents.at(0)->translate(getMarginLeft(), getMarginBottom(), 0, false);
	contents->translate(getMarginLeft() + getPaddingLeft(), getMarginBottom() + getPaddingBottom(), 0, false);
}