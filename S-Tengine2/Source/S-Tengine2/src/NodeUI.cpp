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
#include <shader\ShaderComponentMVP.h>

#include <NumberUtils.h>

ComponentShaderBase * NodeUI::bgShader = nullptr;

NodeUI::NodeUI(BulletWorld * _world, Scene * _scene, bool _mouseEnabled) :
	NodeBulletBody(_world),
	mouse(&Mouse::getInstance()),
	updateState(false),
	isHovered(false),
	isDown(false),
	isActive(false),
	layoutDirty(true),
	scene(_scene),
	onClickFunction(nullptr),
	background(new MeshEntity(MeshFactory::getPlaneMesh())),
	contents(new Transform()),
	boxSizing(kBORDER_BOX),
	mouseEnabled(!_mouseEnabled), // we initialize the variable to the opposite so that setMouseEnabled gets called properly at the end of the constructor
	//bgColour(vox::NumberUtils::randomFloat(-1, 0), vox::NumberUtils::randomFloat(-1, 0), vox::NumberUtils::randomFloat(-1, 0), 1.f)
	bgColour(0.f, 0.f, 0.f, 1.f)
{
	if(bgShader == nullptr){
		bgShader = new ComponentShaderBase(true);
		bgShader->addComponent(new ShaderComponentMVP(bgShader));
		bgShader->addComponent(new ShaderComponentTexture(bgShader));
		bgShader->addComponent(new ShaderComponentTint(bgShader));
		bgShader->addComponent(new ShaderComponentAlpha(bgShader));
		bgShader->compileShader();
	}
	for(unsigned long int i = 0; i < background->mesh->vertices.size(); ++i){
		background->mesh->vertices.at(i).x += 0.5f;
		background->mesh->vertices.at(i).y += 0.5f;
	}
	background->mesh->dirty = true;
	background->setShader(bgShader, true);

	childTransform->addChild(background, true);
	childTransform->addChild(contents, false);
	
	setPadding(0);
	setMargin(0);

	setMouseEnabled(_mouseEnabled);
}

void NodeUI::load(){
	Entity::load();
	bgShader->load();
}
void NodeUI::unload(){
	Entity::unload();
	bgShader->unload();
}

void NodeUI::down(){
	isHovered = true;
	isDown = true;
	
    // do event stuff
	/*if(onDownFunction != nullptr){
		onDownFunction(this);
	}*/
}
void NodeUI::up(){
	if(isHovered){
		click();
	}
	isDown = false;
	
	// do event stuff
	/*if(onUpFunction != nullptr){
		onUpFunction(this);
	}*/
}
void NodeUI::click(){
	// do event stuff
	if(onClickFunction != nullptr){
		onClickFunction(this);
	}
}
void NodeUI::in(){
	isHovered = true;
}
void NodeUI::out(){
	isHovered = false;
}


Transform * NodeUI::addChild(NodeUI* _uiElement){
	layoutDirty = true;
	_uiElement->setMeasuredWidths(this);
	_uiElement->setMeasuredHeights(this);
	return contents->addChild(_uiElement);
}

signed long int NodeUI::removeChild(NodeUI* _uiElement){
	signed long int res = -1;
	if(_uiElement->parents.size() > 0){
		Transform * t = _uiElement->parents.at(0);
		res = contents->removeChild(t);
		t->removeChild(_uiElement);
		delete t;
	
		if(res >= 0){
			layoutDirty = true;
		}
	}
	return res;
}

void NodeUI::setTranslationPhysical(float _x, float _y, float _z, bool _relative){
	background->meshTransform->translate(_x, _y, _z, _relative);
}

bool NodeUI::isMouseEnabled(){
	return mouseEnabled;
}

void NodeUI::setMouseEnabled(bool _mouseEnabled){
	// if nothing has changed, return early
	if(_mouseEnabled == mouseEnabled){
		return;
	}

	if(_mouseEnabled){
		// create the NodeBulletBody collider stuff
		autoResize();
		updateCollider();
	}else{
		// delete the NodeBulletBody collider stuff
		if(shape != nullptr){
			delete shape;
			shape = nullptr;
		}if(body != nullptr){
			world->world->removeRigidBody(body);
			body = nullptr;
		}
	}

	mouseEnabled = _mouseEnabled;
}

void NodeUI::update(Step * _step){
	autoResize();
	if(mouseEnabled){
		updateCollider();

		if(updateState){
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

		updateState = false;
		
		NodeBulletBody::update(_step);
	}
	
	Entity::update(_step);
}

void NodeUI::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	dynamic_cast<ShaderComponentTint *>(dynamic_cast<ComponentShaderBase *>(background->shader)->getComponentAt(2))->setRGB(bgColour.r, bgColour.g, bgColour.b);
	dynamic_cast<ShaderComponentAlpha *>(dynamic_cast<ComponentShaderBase *>(background->shader)->getComponentAt(3))->setAlpha(bgColour.a);
	Entity::render(_matrixStack, _renderOptions);
}

void NodeUI::setMarginLeft(float _margin){
	marginLeft.setSize(_margin);
}
void NodeUI::setMarginRight(float _margin){
	marginRight.setSize(_margin);
}
void NodeUI::setMarginTop(float _margin){
	marginTop.setSize(_margin);
}
void NodeUI::setMarginBottom(float _margin){
	marginBottom.setSize(_margin);
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
	paddingLeft.setSize(_padding);
}

void NodeUI::setPaddingRight(float _padding){
	paddingRight.setSize(_padding);
}

void NodeUI::setPaddingTop(float _padding){
	paddingTop.setSize(_padding);
}

void NodeUI::setPaddingBottom(float _padding){
	paddingBottom.setSize(_padding);
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
	width.measuredSize = getContentsWidth();
	resizeChildrenWidth(this);
}
void NodeUI::setAutoresizeHeight(){
	height.setAutoSize();
	height.measuredSize = getContentsHeight();
	resizeChildrenHeight(this);
}

void NodeUI::setRationalWidth(float _rationalWidth, NodeUI * _root){
	width.setRationalSize(_rationalWidth);
	setMeasuredWidths(_root);
	resizeChildrenWidth(this);
}

void NodeUI::setRationalHeight(float _rationalHeight, NodeUI * _root){
	height.setRationalSize(_rationalHeight);
	setMeasuredHeights(_root);
	resizeChildrenHeight(this);
}

void NodeUI::setPixelWidth(float _pixelWidth){
	if(boxSizing == kBORDER_BOX){
		_pixelWidth -= getMarginLeft() + getPaddingLeft() + getPaddingRight() + getMarginRight();
	}
	width.setPixelSize(_pixelWidth);
	resizeChildrenWidth(this);
}
void NodeUI::setPixelHeight(float _pixelHeight){
	if(boxSizing == kBORDER_BOX){
		_pixelHeight -= getMarginBottom() + getPaddingBottom() + getPaddingTop() + getMarginTop();
	}
	height.setPixelSize(_pixelHeight);
	resizeChildrenHeight(this);
}

void NodeUI::resizeChildrenWidth(NodeUI * _root){
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * nui = dynamic_cast<NodeUI *>(trans->children.at(0));
				if(nui != nullptr){
					nui->setMeasuredWidths(_root);
				}
			}
		}
	}
}

void NodeUI::resizeChildrenHeight(NodeUI * _root){
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		Transform * trans = dynamic_cast<Transform *>(contents->children.at(i));
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				NodeUI * nui = dynamic_cast<NodeUI *>(trans->children.at(0));
				if(nui != nullptr){
					nui->setMeasuredHeights(_root);
				}
			}
		}
	}
}

void NodeUI::setMeasuredWidths(NodeUI * _root){
	
	float rootWidth = 0.f;
	if(_root != nullptr){
		rootWidth = _root->getWidth();
	}

	if(marginLeft.sizeMode == kRATIO){
		marginLeft.measuredSize = rootWidth * marginLeft.rationalSize;
	}if(paddingLeft.sizeMode == kRATIO){
		paddingLeft.measuredSize = rootWidth * paddingLeft.rationalSize;
	}if(paddingRight.sizeMode == kRATIO){
		paddingRight.measuredSize = rootWidth * paddingRight.rationalSize;
	}if(marginRight.sizeMode == kRATIO){
		marginRight.measuredSize = rootWidth * marginRight.rationalSize;
	}if(width.sizeMode == kRATIO){
		width.measuredSize = rootWidth * width.rationalSize;
		if(boxSizing == kBORDER_BOX){
			width.measuredSize -= marginLeft.getSize() + paddingLeft.getSize() + paddingRight.getSize() + marginRight.getSize();
		}
	}

	if(width.sizeMode == kAUTO){
		// if the width of this node is auto-sized, the children's width has to be based on the element one level above this one
		// to avoid the conflict between an auto-sized container and a rational-sized child
		resizeChildrenWidth(_root);
	}else{
		resizeChildrenWidth(this);
	}
}
void NodeUI::setMeasuredHeights(NodeUI * _root){
	float rootHeight = 0.f;
	if(_root != nullptr){
		rootHeight = _root->getHeight();
	}
	if(marginBottom.sizeMode == kRATIO){
		marginBottom.measuredSize = rootHeight * marginBottom.rationalSize;
	}if(paddingBottom.sizeMode == kRATIO){
		paddingBottom.measuredSize = rootHeight * paddingBottom.rationalSize;
	}if(paddingTop.sizeMode == kRATIO){
		paddingTop.measuredSize = rootHeight * paddingTop.rationalSize;
	}if(marginTop.sizeMode == kRATIO){
		marginTop.measuredSize = rootHeight * marginTop.rationalSize;
	}if(height.sizeMode == kRATIO){
		height.measuredSize = rootHeight * height.rationalSize;
		if(boxSizing == kBORDER_BOX){
			height.measuredSize -= marginBottom.getSize() + paddingBottom.getSize() + paddingTop.getSize() + marginTop.getSize();
		}
	}

	if(height.sizeMode == kAUTO){
		// if the height of this node is auto-sized, the children's height has to be based on the element one level above this one
		// to avoid the conflict between an auto-sized container and a rational-sized child
		resizeChildrenHeight(_root);
	}else{
		resizeChildrenHeight(this);
	}
}


void NodeUI::setBackgroundColour(float _r, float _g, float _b, float _a){
	bgColour.r = _r;
	bgColour.g = _g;
	bgColour.b = _b;
	bgColour.a = _a;
}

float NodeUI::getMarginLeft(){
	return marginLeft.getSize();
}float NodeUI::getMarginRight(){
	return marginRight.getSize();
}float NodeUI::getMarginTop(){
	return marginTop.getSize();
}float NodeUI::getMarginBottom(){
	return marginBottom.getSize();
}

float NodeUI::getPaddingLeft(){
	return paddingLeft.getSize();
}float NodeUI::getPaddingRight(){
	return paddingRight.getSize();
}float NodeUI::getPaddingTop(){
	return paddingTop.getSize();
}float NodeUI::getPaddingBottom(){
	return paddingBottom.getSize();
}

float NodeUI::getWidth(){
	return width.getSize();
}float NodeUI::getHeight(){
	return height.getSize();
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

TriMesh * NodeUI::colliderMesh = nullptr;
void NodeUI::updateCollider(){
	glm::mat4 mat = background->meshTransform->getCumulativeModelMatrix();
	
	glm::vec4 verts[6] = {
		mat * glm::vec4(0,						1,0,1),
		mat * glm::vec4(1,	1,0,1),
		mat * glm::vec4(1,	0,0,1),
		mat * glm::vec4(1,	0,0,1),
		mat * glm::vec4(0,						0,0,1),
		mat * glm::vec4(0,						0,0,1)
	};

	if(colliderMesh == nullptr){
		colliderMesh = new TriMesh();
		for(unsigned long int i = 0; i < 6; ++i){
			colliderMesh->pushVert(Vertex(verts[i].x, verts[i].y, verts[i].z));
		}
	}else{
		for(unsigned long int i = 0; i < 6; ++i){
			colliderMesh->vertices.at(i).x = verts[i].x;
			colliderMesh->vertices.at(i).y = verts[i].y;
			colliderMesh->vertices.at(i).z = verts[i].z;
		}
	}

	if(shape != nullptr){
		delete shape;
		shape = nullptr;
	}
	setColliderAsMesh(colliderMesh, true);

	
	if(body != nullptr){
		world->world->removeRigidBody(body);
		body->setCollisionShape(shape);
		world->world->addRigidBody(body);
	}else{
		createRigidBody(0);
	}
}

void NodeUI::autoResize(){
	if(width.sizeMode == kAUTO){
		width.measuredSize = getContentsWidth();
	}
	if(height.sizeMode == kAUTO){
		height.measuredSize = getContentsHeight();
	}
	// Adjust the size of the background
	background->parents.at(0)->scale(getWidth(true, false), getHeight(true, false), 1.0f, false);
	repositionChildren();
}
float NodeUI::getContentsWidth(){
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
	return w;
}
float NodeUI::getContentsHeight(){
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
	return h;
}

void NodeUI::repositionChildren(){
	glm::vec3 bpos(0);
	glm::vec3 cpos(0);

	bpos.x = getMarginLeft();
	cpos.x = getMarginLeft() + getPaddingLeft();

	bpos.y = getMarginBottom();
	cpos.y = getMarginBottom() + getPaddingBottom();

	background->parents.at(0)->translate(bpos, false);
	contents->translate(cpos, false);
}


void NodeUI::setUpdateState(bool _newState){
	updateState = _newState;
}