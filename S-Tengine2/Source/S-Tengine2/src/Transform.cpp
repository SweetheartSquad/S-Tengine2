#pragma once

#include <Transform.h>
#include <node\NodeResource.h>
#include <MatrixStack.h>

Transform::Transform():
	translationVector(0.f, 0.f, 0.f),
	scaleVector(1.f, 1.f, 1.f),
	orientation(1.f, 0.f, 0.f, 0.f),
	tDirty(true),
	sDirty(true),
	oDirty(true),
	mDirty(true),
	isIdentity(true)
{
}

Transform::~Transform(){
	//while(children.size() > 0){
		//NodeResource * nr = dynamic_cast<NodeResource *>(children.back());
		//if(nr != nullptr){
		//	nr->decrementAndDelete();
		//}else{
		//	delete children.back();
		//}
		//children.pop_back();
	//}
}

void Transform::makeCumulativeModelMatrixDirty(){
	NodeChild::makeCumulativeModelMatrixDirty();
	for(NodeChild * child : children){
		child->makeCumulativeModelMatrixDirty();
	}
}


glm::mat4 Transform::getCumulativeModelMatrix(){
	if(cumulativeModelMatrixDirty){
		Transform * p = parent;
		if(p != nullptr){
			cumulativeModelMatrix = p->getCumulativeModelMatrix() * getModelMatrix();
		}else{
			cumulativeModelMatrix = getModelMatrix();
		}
		cumulativeModelMatrixDirty = false;
	}
	return cumulativeModelMatrix;
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

glm::vec3 Transform::getWorldPos(){
	if(parent == nullptr){
		return getTranslationVector();
	}
	if(cumulativeModelMatrixDirty){
		glm::vec4 res(getTranslationVector(), 1);
		if(parent != nullptr){
			res = parent->getCumulativeModelMatrix() * res;
		}

		worldPos = glm::vec3(res);
		cumulativeModelMatrixDirty = false;
	}
	return worldPos;
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
	// save previous matrix state
	_matrixStack->pushMatrix();
	// apply the transform's matrix
	if(!isIdentity){
		_matrixStack->applyMatrix(getModelMatrix());
	}

	// render all of the transform's children
	for(unsigned long int i = 0; i < children.size(); i++){
		NodeRenderable * nr = dynamic_cast<NodeRenderable *>(children.at(i));
		if(nr != nullptr){
			nr->render(_matrixStack, _renderOptions);	
		}
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


void Transform::addChildAtIndex(NodeChild * _child, int _index){
	children.insert(children.begin() + _index, _child);
	_child->setParent(this);
}

void Transform::removeChildAtIndex(int _index){
	children.erase(children.begin() + _index);
	children.at(_index)->setParent(nullptr);
}


unsigned long int Transform::removeChild(NodeChild * _child){
	for(unsigned long int i = 0; i < children.size(); ++i){
		if(_child == children.at(i)){
			children.erase(children.begin() + i);
			_child->setParent(nullptr);
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


unsigned long int Transform::calculateDepth(){
	unsigned long int depth = 0;
	Transform * t = parent;
	while(t != nullptr){
		t = t->parent;
		depth += 1;
	}
	return depth;
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

bool Transform::addChild(NodeChild * _child){
	// Check to see if the child is one of the ancestors of this node
	bool error = hasAncestor(dynamic_cast<Transform *>(_child));

	if(!error){
		// Remove the first instance of the child in the current list of children
		for (unsigned long int i = 0; i < children.size(); ++i){
			if (_child == children.at(i)){
				children.erase(children.begin() + i);
				break;
			}
		}

		// Add the child to the list of children and set it's parent to this
		children.push_back(_child);
		_child->setParent(this);
		return true;
	}else{
		// Error Message: Cannot parent a node to one of its descendants
		return false;
	}
}



void Transform::printHierarchy(unsigned long int _startDepth){
	NodeChild::printHierarchy(_startDepth);
	for(NodeChild * c : children){
		c->printHierarchy(_startDepth+1);
	}
}