#pragma once

#include <Transform.h>
#include <node\NodeResource.h>
#include <MatrixStack.h>
#include <RenderOptions.h>
#include <MeshEntity.h>
#include <MeshInterface.h>
#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentTexture.h>

MeshInterface * Transform::transformIndicator = nullptr;
BaseComponentShader * Transform::transformShader = nullptr;
bool Transform::staticInit = true;
bool Transform::drawTransforms = false;

Transform::Transform():
	translationVector(0.f, 0.f, 0.f),
	scaleVector(1.f, 1.f, 1.f),
	orientation(1.f, 0.f, 0.f, 0.f),
	tDirty(true),
	sDirty(true),
	oDirty(true),
	mDirty(true),
	isIdentity(true),
	cumulativeModelMatrix(1)
{
	if(staticInit){
		staticInit = false;
		transformShader = new BaseComponentShader(false);
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
	NodeChild::makeCumulativeModelMatrixDirty();
	for(NodeChild * child : children){
		child->makeCumulativeModelMatrixDirty();
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
	assert(parents.size() == 0); // transforms should only ever have one parent you dumbo
	NodeChild::addParent(_parent);
}


void Transform::scale(float _scaleX, float _scaleY, float _scaleZ, bool _relative){
	scale(glm::vec3(_scaleX, _scaleY, _scaleZ), _relative);
}

void Transform::scale(glm::vec3 _scale, bool relative){
	if(relative){
		scaleVector *= _scale;
	}else{
		scaleVector = _scale;
	}
	sDirty = true;
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
	mDirty = true;
	isIdentity = false;
	makeCumulativeModelMatrixDirty();
}

glm::mat4 Transform::getTranslationMatrix(){
	if(tDirty){
		tMatrix = glm::translate(translationVector);
		tDirty = false;
	}
	return tMatrix;
}

glm::mat4 Transform::getScaleMatrix(){
	if(sDirty){
		sMatrix = glm::scale(scaleVector);
		sDirty = false;
	}
	return sMatrix;
}

glm::mat4 Transform::getOrientationMatrix(){
	if(oDirty){
		oMatrix = glm::toMat4(orientation);
		oDirty = false;
	}
	return oMatrix;
}


glm::mat4 Transform::getModelMatrix(){
	if(mDirty){
		mMatrix = getTranslationMatrix() * getOrientationMatrix() * getScaleMatrix();
		mDirty = false;
	}
	return mMatrix;
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


void Transform::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	// don't bother doing any work if we aren't rendering anyway
	if(!visible){
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
		if(nr != nullptr && nr->isVisible()){
			nr->render(_matrixStack, _renderOptions);	
		}
	}
	if(drawTransforms){
		_renderOptions->shader = transformShader;
		transformIndicator->render(_matrixStack, _renderOptions);
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
	assert(!hasAncestor(dynamic_cast<Transform *>(_child)));

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
	assert(!hasAncestor(dynamic_cast<Transform *>(_child)));
	
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

void Transform::printHierarchy(unsigned long int _startDepth){
	NodeChild::printHierarchy(_startDepth);
	for(NodeChild * c : children){
		c->printHierarchy(_startDepth+1);
	}
}