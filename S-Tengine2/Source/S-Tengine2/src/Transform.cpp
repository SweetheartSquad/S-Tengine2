#pragma once

#include <Transform.h>
#include <node\NodeResource.h>
#include <MatrixStack.h>
#include <RenderOptions.h>
#include <MeshEntity.h>
#include <MeshInterface.h>
#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentMVP.h>
#include <Log.h>
#include <AntTweakBar.h>
#include <ctime>
#include <NumberUtils.h>

MeshInterface * Transform::transformIndicator = nullptr;
ComponentShaderBase * Transform::transformShader = nullptr;
bool Transform::staticInit = true;
bool Transform::drawTransforms = false;

Transform::Transform():
	translationVector(0.f, 0.f, 0.f),
	scaleVector(1.f, 1.f, 1.f),
	orientation(1.f, 0.f, 0.f, 0.f),
	tDirty(true),
	sDirty(true),
	oDirty(true),
	osDirty(true),
	mDirty(true),
	isIdentity(true),
	cumulativeModelMatrix(1)
{
	ptrTransform = this;

	if(staticInit){
		staticInit = false;
		transformShader = new ComponentShaderBase(true);
		transformShader->addComponent(new ShaderComponentMVP(transformShader));
		transformShader->addComponent(new ShaderComponentTexture(transformShader));
		transformShader->compileShader();
		transformShader->load();
		transformShader->incrementReferenceCount();
		transformShader->nodeName = "Transform Indicator Shader";

		transformIndicator = new MeshInterface(GL_LINES, GL_STATIC_DRAW);
		transformIndicator->pushVert(Vertex(0,0,0, 1,0,0,1));
		transformIndicator->pushVert(Vertex(1,0,0, 1,0,0,1));
		transformIndicator->pushVert(Vertex(0,0,0, 0,1,0,1));
		transformIndicator->pushVert(Vertex(0,1,0, 0,1,0,1));
		transformIndicator->pushVert(Vertex(0,0,0, 0,0,1,1));
		transformIndicator->pushVert(Vertex(0,0,1, 0,0,1,1));
		transformIndicator->configureDefaultVertexAttributes(transformShader);
		transformIndicator->incrementReferenceCount();
		transformIndicator->nodeName = "Transform Indicator";
	}
}

Transform::~Transform(){
	while(!children.empty()){
		if(NodeResource * nr = children.back()->asNodeResource()){
			nr->decrementAndDelete();
		}else{
			delete children.back();
		}
		children.pop_back();
	}
	children.clear();
}

void Transform::makeCumulativeModelMatrixDirty(){
	if(!cumulativeModelMatrixDirty){
		NodeChild::makeCumulativeModelMatrixDirty();
		for(NodeChild * child : children){
			child->makeCumulativeModelMatrixDirty();
		}
	}
}

glm::mat4 Transform::getCumulativeModelMatrix(){
	// if the transform has no parent, return the identity matrix
	if(parents.size() == 0){
		return getModelMatrix();
	}
	if(cumulativeModelMatrixDirty){
		cumulativeModelMatrix = firstParent()->getCumulativeModelMatrix() * getModelMatrix();
		cumulativeModelMatrixDirty = false;
	}
	return cumulativeModelMatrix;
}

void Transform::addParent(Transform * _parent){
	if(parents.size() != 0){
		ST_LOG_ERROR_V("Transform should never have more than one parent");
		assert(false);
	}
	NodeChild::addParent(_parent);
}

Transform * const Transform::scale(float _scaleX, float _scaleY, float _scaleZ, bool _relative){
	scale(glm::vec3(_scaleX, _scaleY, _scaleZ), _relative);
	return this;
}

Transform * const Transform::scale(float _scale, bool _relative){
	scale(glm::vec3(_scale, _scale, _scale), _relative);
	return this;
}

Transform * const Transform::scale(glm::vec3 _scale, bool relative){
	if(relative){
		scaleVector *= _scale;
	}else{
		scaleVector = _scale;
	}
	sDirty = true;
	osDirty = true;
	mDirty = true;
	isIdentity = false;
	makeCumulativeModelMatrixDirty();
	return this;
}

Transform * const Transform::translate(float _translateX, float _translateY, float _translateZ, bool _relative){
	translate(glm::vec3(_translateX, _translateY, _translateZ), _relative);
	return this;
}

Transform * const Transform::translate(glm::vec3 _translate, bool _relative){
	if(_relative){
		translationVector += _translate;
	}else{
		translationVector = _translate;
	}
	tDirty = true;
	mDirty = true;
	isIdentity = false;
	makeCumulativeModelMatrixDirty();
	return this;
}

Transform * const Transform::rotate(glm::quat _rotation, CoordinateSpace _space){
	switch(_space){
	case kWORLD:
		setOrientation(_rotation * orientation);
		break;
	case kOBJECT:
		setOrientation(orientation * _rotation);
		break;
	}
	return this;
}

Transform * const Transform::rotate(float _angle, float _x, float _y, float _z, CoordinateSpace _space){
	rotate(glm::quat(glm::angleAxis(_angle, glm::vec3(_x, _y, _z))), _space);
	return this;
}
Transform * const Transform::setOrientation(glm::quat _orientation){
	orientation = _orientation;
	oDirty = true;
	osDirty = true;
	mDirty = true;
	isIdentity = false;
	makeCumulativeModelMatrixDirty();
	return this;
}

Transform * const Transform::lookAt(glm::vec3 _pos, glm::vec3 _up, float _interpolation){
	Transform t;
	t.parents.push_back(this);
	glm::mat4 m = glm::lookAt(_pos, t.getWorldPos(), _up);
	glm::quat q(glm::transpose(m));
	setOrientation(glm::slerp(getOrientationQuat(), q, _interpolation));
	return this;
}

const glm::mat4 & Transform::getTranslationMatrix(){
	if(tDirty){
		tMatrix = glm::translate(translationVector);
		tDirty = false;
	}
	return tMatrix;
}

const glm::mat4 & Transform::getScaleMatrix(){
	if(sDirty){
		sMatrix = glm::scale(scaleVector);
		sDirty = false;
	}
	return sMatrix;
}

const glm::mat4 & Transform::getOrientationMatrix(){
	if(oDirty){
		oMatrix = glm::toMat4(orientation);
		oDirty = false;
	}
	return oMatrix;
}

const glm::mat4 & Transform::getOrientationScaleMatrix(){
	if(osDirty){
		osMatrix = getOrientationMatrix() * getScaleMatrix();
		osDirty = false;
	}
	return osMatrix;
}

const glm::mat4 & Transform::getModelMatrix(){
	if(mDirty){
		mMatrix = getTranslationMatrix() * getOrientationScaleMatrix();
		mDirty = false;
	}
	return mMatrix;
}

void Transform::resetTranslation() {
	translationVector = glm::vec3(0.f, 0.f, 0.f);
}

void Transform::resetOrientation() {
	orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
}

void Transform::resetScale() {
	scaleVector = glm::vec3(1.f, 1.f, 1.f);
}

void Transform::reset(){
	translate(glm::vec3(0.f, 0.f, 0.f), false);
	scale(glm::vec3(1.f, 1.f, 1.f), false);
	setOrientation(glm::quat(1.f, 0.f, 0.f, 0.f));
	isIdentity = true;
	makeCumulativeModelMatrixDirty();
}

glm::vec3 Transform::getTranslationVector() const {
	return translationVector;
}
glm::vec3 Transform::getScaleVector() const {
	return scaleVector;
}
glm::quat Transform::getOrientationQuat() const {
	return orientation;
}

void Transform::update(Step * _step){
	for(unsigned long int i = 0; i < children.size(); ++i){
		if(children.at(i)->asNodeUpdatable() != nullptr){
			children.at(i)->asNodeUpdatable()->update(_step);
		}
	}
}

void Transform::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	// don't bother doing any work if we aren't rendering anyway
	if(!isVisible()){
		return;
	}

	// save previous matrix state
	_matrixStack->pushMatrix();
	// apply the transform's matrix
	if(!isIdentity){
		_matrixStack->applyMatrix(getModelMatrix());
	}

	// render all of the transform's children
	for(unsigned long int i = 0; i < children.size(); i++){
		if(children.at(i)->asNodeRenderable() != nullptr){
			children.at(i)->asNodeRenderable()->render(_matrixStack, _renderOptions);
		}
	}
	if(drawTransforms){
		Shader * prev = _renderOptions->shader;
		_renderOptions->shader = transformShader;

		float prevLineWidth;
		glGetFloatv(GL_LINE_WIDTH, &prevLineWidth);
		glLineWidth(5);
		transformIndicator->render(_matrixStack, _renderOptions);
		glLineWidth(prevLineWidth);
		_renderOptions->shader = prev;
	}

	// restore previous matrix state
	_matrixStack->popMatrix();
}

void Transform::unload(){
	for(NodeChild * child : children){
		NodeLoadable * nl = child->asNodeLoadable();
		if(nl != nullptr){
			nl->unload();
		}
	}

	NodeLoadable::unload();
}

void Transform::load(){
	for(NodeChild * child : children){
		NodeLoadable * nl = child->asNodeLoadable();
		if(nl != nullptr){
			nl->load();
		}
	}

	NodeLoadable::load();
}

Transform * const Transform::addChild(NodeChild * const _child, bool _underNewTransform){
	// Check to see if the child is one of the ancestors of this node
	// (Cannot parent a node to one of its descendants)
	if(hasAncestor(_child->asTransform())) {
		ST_LOG_ERROR_V("Cannot parent a node to one of it's ancestors");
		assert(false);
	}

	if(_underNewTransform){
		Transform * t = new Transform();
		t->addChild(_child, false);
		children.push_back(t);
		t->addParent(this);
		return t;
	}

	removeChild(_child);
	children.push_back(_child);
	_child->addParent(this);
	return nullptr;
}

Transform * const Transform::addChildAtIndex(NodeChild * const _child, int _index, bool _underNewTransform){
	// Check to see if the child is one of the ancestors of this node
	// (Cannot parent a node to one of its descendants)
	if(hasAncestor(_child->asTransform())) {
		ST_LOG_ERROR_V("Cannot parent a node to one of it's ancestors");
		assert(false);
	}

	// if negative, count from the end
	if(_index < 0){
		_index = children.size() + _index;
	}

	if(_underNewTransform){
		Transform * t = new Transform();
		t->addChild(_child, false);
		children.insert(children.begin() + _index, t);
		t->addParent(this);
		return t;
	}

	removeChild(_child);
	children.insert(children.begin() + _index, _child);
	_child->addParent(this);
	return nullptr;
}

void Transform::removeChildAtIndex(int _index){
	children.erase(children.begin() + _index);
	children.at(_index)->removeParent(this);
}

unsigned long int Transform::removeChild(NodeChild * const _child){
	for(signed long int i = children.size()-1; i >= 0; --i){
		if(_child == children.at(i)){
			children.erase(children.begin() + i);
			_child->removeParent(this);
			return i;
		}
	}
	return -1;
}

bool Transform::hasChild(const NodeChild * const _child) const{
	if(_child == nullptr){
		return false;
	}
	for (unsigned long int i = 0; i < children.size(); ++i){
		if (_child == children.at(i)){
			return true;
		}
	}
	return false;
}

bool Transform::hasDescendant(const NodeChild * const _child) const{
	if(_child == nullptr){
		return false;
	}
	for (unsigned long int i = 0; i < children.size(); ++i){
		if (_child == children.at(i)){
			return true;
		}else{
			Transform * t = children.at(i)->asTransform();
			if(t != nullptr){
				if(t->hasDescendant(_child)){
					return true;
				}
			}
		}
	}
	return false;
}

void Transform::doRecursively(std::function<void(Node *, void * args[])> _toDo, void * _args[]){
	_toDo(this, _args);
	for(unsigned long int i = 0; i < children.size(); i++){
		Transform * t = children.at(i)->asTransform();
		if(t != nullptr){
			t->doRecursively(_toDo, _args);
		}else{
			_toDo(children.at(i), _args);
		}
	}
}

void Transform::deleteRecursively(Transform * const _node){
	while (_node->children.size() > 0){
		Transform * t = _node->children.back()->asTransform();
		if(t != nullptr){
			deleteRecursively(t);
		}
		_node->children.pop_back();
	}
	delete _node;
}

void Transform::printHierarchy(unsigned long int _startDepth, bool _last, std::vector<unsigned long int> & _p){
	NodeChild::printHierarchy(_startDepth, _last, _p);
	if(!_last){
		_p.push_back(_startDepth);
	}
	for(NodeChild * c : children){
		c->printHierarchy(_startDepth+1, c == children.back(), _p);
	}
	if(!_last){
		_p.pop_back();
	}
}

TwBar * Transform::createAntTweakBarWindow(std::string _name) {
	if(sweet::antTweakBarInititialized){
		// Names must be unique
		double t = sweet::step.time + sweet::NumberUtils::randomFloat(1, 1000);
		TwBar * bar = TwNewBar(_name.c_str());
		TwAddVarCB(bar, std::string(std::to_string(t) + "trans").c_str(),  TW_TYPE_DIR3F, twSetTranslation, twGetTranslation, this, "label=Translation");
		TwAddVarCB(bar, std::string(std::to_string(t) + "scale").c_str(),  TW_TYPE_DIR3F, twSetScale, twGetScale, this, "label=Scale");
		TwAddVarCB(bar, std::string(std::to_string(t) + "rotate").c_str(), TW_TYPE_QUAT4F, twSetRotation, twGetRotation, this, "label=Rotation");
		return bar;
	}
	return nullptr;
}

void Transform::addToAntTweakBar(TwBar* _bar, std::string _name) {
	if(sweet::antTweakBarInititialized){
		// Names must be unique
		TwAddSeparator(_bar, _name.c_str(), std::string("label="+_name).c_str());
		double t = sweet::step.time + sweet::NumberUtils::randomFloat(1, 1000);
		TwAddVarCB(_bar, std::string(std::to_string(t) + "trans").c_str(),  TW_TYPE_DIR3F, twSetTranslation, twGetTranslation, this, "label=Translation");
		TwAddVarCB(_bar, std::string(std::to_string(t) + "scale").c_str(),  TW_TYPE_DIR3F, twSetScale, twGetScale, this, "label=Scale");
		TwAddVarCB(_bar, std::string(std::to_string(t) + "rotate").c_str(), TW_TYPE_QUAT4F, twSetRotation, twGetRotation, this, "label=Rotation");
	}
}

void TW_CALL Transform::twSetTranslation(const void * value, void * clientData) {
	((Transform *)clientData)->translate(((float*)value)[0], ((float*)value)[1], ((float*)value)[2], false);
}

void TW_CALL Transform::twGetTranslation(void * value, void * clientData) {
	glm::vec3 trans = ((Transform *)clientData)->getTranslationVector();
	((float *)value)[0] = trans.x;
	((float *)value)[1] = trans.y;
	((float *)value)[2] = trans.z;
}

void TW_CALL Transform::twSetScale(const void * value, void * clientData) {
	((Transform *)clientData)->scale(((float*)value)[0], ((float*)value)[1], ((float*)value)[2], false);
}

void TW_CALL Transform::twGetScale(void * value, void * clientData) {
	glm::vec3 scale = ((Transform *)clientData)->getScaleVector();
	((float *)value)[0] = scale.x;
	((float *)value)[1] = scale.y;
	((float *)value)[2] = scale.z;
}

void TW_CALL Transform::twSetRotation(const void * value, void * clientData) {
	glm::quat rot = ((Transform *)clientData)->getOrientationQuat();
	rot.x = ((float*)value)[0];
	rot.y = ((float*)value)[1];
	rot.z = ((float*)value)[2];
	rot.w = ((float*)value)[3];
	((Transform *)clientData)->setOrientation(rot);
}

void TW_CALL Transform::twGetRotation(void * value, void * clientData) {
	glm::quat rot = ((Transform *)clientData)->getOrientationQuat();
	((float *)value)[0] = rot.x;
	((float *)value)[1] = rot.y;
	((float *)value)[2] = rot.z;
	((float *)value)[3] = rot.w;
}