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
	if(staticInit){
		staticInit = false;
		transformShader = new ComponentShaderBase(false);
		transformShader->addComponent(new ShaderComponentMVP(transformShader));
		transformShader->addComponent(new ShaderComponentTexture(transformShader));
		transformShader->compileShader();
		transformShader->load();
		transformIndicator = new MeshInterface(GL_LINES, GL_STATIC_DRAW);
		transformIndicator->pushVert(Vertex(0,0,0, 1,0,0,1));
		transformIndicator->pushVert(Vertex(1,0,0, 1,0,0,1));
		transformIndicator->pushVert(Vertex(0,0,0, 0,1,0,1));
		transformIndicator->pushVert(Vertex(0,1,0, 0,1,0,1));
		transformIndicator->pushVert(Vertex(0,0,0, 0,0,1,1));
		transformIndicator->pushVert(Vertex(0,0,1, 0,0,1,1));
		transformIndicator->configureDefaultVertexAttributes(transformShader);
	}
}

Transform::~Transform(){
	while(children.size() > 0){
		NodeResource * nr = dynamic_cast<NodeResource *>(children.back());
		if(nr != nullptr){
			nr->decrementAndDelete();
		}else{
			delete children.back();
		}
		children.pop_back();
	}
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
		cumulativeModelMatrix = parents.at(0)->getCumulativeModelMatrix() * getModelMatrix();
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


void Transform::scale(float _scaleX, float _scaleY, float _scaleZ, bool _relative){
	scale(glm::vec3(_scaleX, _scaleY, _scaleZ), _relative);
}

void Transform::scale(float _scale, bool _relative){
	scale(glm::vec3(_scale, _scale, _scale), _relative);
}

void Transform::scale(glm::vec3 _scale, bool relative){
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
}

void Transform::translate(float _translateX, float _translateY, float _translateZ, bool _relative){
	translate(glm::vec3(_translateX, _translateY, _translateZ), _relative);
}

void Transform::translate(glm::vec3 _translate, bool _relative){
	if(_relative){
		translationVector += _translate;
	}else{
		translationVector = _translate;
	}
	tDirty = true;
	mDirty = true;
	isIdentity = false;
	makeCumulativeModelMatrixDirty();
}

void Transform::rotate(glm::quat _rotation, CoordinateSpace _space){
	switch(_space){
	case kWORLD:
		setOrientation(_rotation * orientation);
		break;
	case kOBJECT:
		setOrientation(orientation * _rotation);
		break;
	}
}

void Transform::rotate(float _angle, float _x, float _y, float _z, CoordinateSpace _space){
	rotate(glm::quat(glm::angleAxis(_angle, glm::vec3(_x, _y, _z))), _space);
}
void Transform::setOrientation(glm::quat _orientation){
	orientation = _orientation;
	oDirty = true;
	osDirty = true;
	mDirty = true;
	isIdentity = false;
	makeCumulativeModelMatrixDirty();
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

glm::vec3 Transform::getTranslationVector(){
	return translationVector;
}
glm::vec3 Transform::getScaleVector(){
	return scaleVector;
}
glm::quat Transform::getOrientationQuat(){
	return orientation;
}

void Transform::update(Step * _step){
	for(unsigned long int i = 0; i < children.size(); ++i){
		NodeUpdatable * nu = dynamic_cast<NodeUpdatable *>(children.at(i));
		if(nu != nullptr){
			nu->update(_step);	
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
		NodeRenderable * nr = dynamic_cast<NodeRenderable *>(children.at(i));
		if(nr != nullptr /*&& nr->isVisible()*/){
			nr->render(_matrixStack, _renderOptions);
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
		NodeLoadable * nl = dynamic_cast<NodeLoadable *>(child);
		if(nl != nullptr){
			nl->unload();
		}
	}
	
	NodeLoadable::unload();
}

void Transform::load(){
	for(NodeChild * child : children){
		NodeLoadable * nl = dynamic_cast<NodeLoadable *>(child);
		if(nl != nullptr){
			nl->load();	
		}
	}
	
	NodeLoadable::load();
}

Transform * Transform::addChild(NodeChild * _child, bool _underNewTransform){
	// Check to see if the child is one of the ancestors of this node
	// (Cannot parent a node to one of its descendants)
	if(hasAncestor(dynamic_cast<Transform *>(_child))) {
		ST_LOG_ERROR_V("Cannot parent a node to one of it's ancestors");
		assert(false);
	}

	Transform * t = nullptr;
	if(_underNewTransform){
		t = new Transform();
		t->addChild(_child, false);
		_child = t;
	}else{
		removeChild(_child);
	}

	// Add the child to the list of children and set it's parent to this
	children.push_back(_child);
	_child->addParent(this);
	return t;
}

Transform * Transform::addChildAtIndex(NodeChild * _child, int _index, bool _underNewTransform){
	// Check to see if the child is one of the ancestors of this node
	// (Cannot parent a node to one of its descendants)
	if(hasAncestor(dynamic_cast<Transform *>(_child))) {
		ST_LOG_ERROR_V("Cannot parent a node to one of it's ancestors");
		assert(false);
	}

	Transform * t = nullptr;
	if(_underNewTransform){
		Transform * t = new Transform();
		t->addChild(_child, false);
		_child = t;
	}else{
		removeChild(_child);
	}

	children.insert(children.begin() + _index, _child);
	_child->addParent(this);
	return t;
}

void Transform::removeChildAtIndex(int _index){
	children.erase(children.begin() + _index);
	children.at(_index)->removeParent(this);
}

unsigned long int Transform::removeChild(NodeChild * _child){
	for(signed long int i = children.size()-1; i >= 0; --i){
		if(_child == children.at(i)){
			children.erase(children.begin() + i);
			_child->removeParent(this);
			return i;
		}
	}
	return -1;
}

bool Transform::hasChild(NodeChild * _child){
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

bool Transform::hasDescendant(NodeChild *_child) {
	if(_child == nullptr){
		return false;
	}
	for (unsigned long int i = 0; i < children.size(); ++i){
		if (_child == children.at(i)){
			return true;
		}else{
			Transform * t = dynamic_cast<Transform *>(children.at(i));
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
		Transform * t = dynamic_cast<Transform *>(children.at(i));
		if(t != nullptr){
			t->doRecursively(_toDo, _args);
		}else{
			_toDo(children.at(i), _args);
		}
	}
}

void Transform::deleteRecursively(Transform * _node){
	while (_node->children.size() > 0){
		Transform * t = dynamic_cast<Transform *>(_node->children.back());
		if(t != nullptr){
			deleteRecursively(t);
		}
		_node->children.pop_back();
	}
	delete _node;
	_node = nullptr;
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
