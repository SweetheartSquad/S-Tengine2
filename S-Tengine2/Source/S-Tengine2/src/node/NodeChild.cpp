﻿#pragma once

#include <node\NodeChild.h>
#include <Transform.h>
#include <Log.h>

NodeChild::NodeChild() :
	worldPos(0),
	cumulativeModelMatrixDirty(true)
{
	nodeType |= NodeType::kNODE_CHILD;
	ptrNodeChild = this;
}

NodeChild::~NodeChild(){
	parents.clear();
}

void NodeChild::makeCumulativeModelMatrixDirty(){
	// do nothing
	cumulativeModelMatrixDirty = true;
}

bool NodeChild::hasAncestor(const Transform * const _parent) const{
	if(_parent == nullptr){
		return false;
	}
	for(Transform * p : parents){
		if(p == _parent) {
			return true;
		}
#pragma warning(suppress: 6011) // children should never have nullptr in their parent list
		if(p->hasAncestor(_parent)){
			return true;
		}
	}
	return false;
}

glm::vec3 NodeChild::getWorldPos(unsigned long int _parent){
	// if the node has no parent, return a zero-vector
	if(parents.size() <= _parent){
		return glm::vec3(0);
	}

	Transform * p = parents.at(_parent);
	// if the cumulative model matrix is out-of-date, then so is the stored world position
	//if(cumulativeModelMatrixDirty){
	// find the first non-zero ancestor translation vector
	glm::vec3 res(0);
	do{
		res = p->getTranslationVector();
		if(p->parents.size() == 0){
			break;
		}
		p = p->parents.at(0);
	}while(res == glm::vec3(0));

	// apply the cumulative matrix of its parent to the vector
	if(p->parents.size() > 0){
		res = glm::vec3(p->parents.at(0)->getCumulativeModelMatrix() * glm::vec4(res, 1));
	}

	worldPos = res;
	//cumulativeModelMatrixDirty = false;
	//}
	return worldPos;
}

void NodeChild::addParent(Transform * const _parent){
	parents.push_back(_parent);
	makeCumulativeModelMatrixDirty();
}

void NodeChild::removeParent(Transform * const _parent){
	for(signed long int i = parents.size()-1; i >= 0; --i){
		if(parents.at(i) == _parent){
			parents.erase(parents.begin() + i);
			return;
		}
	}
}

Transform* NodeChild::firstParent() {
	if(parents.size() > 0) {
		return(parents.at(0));
	}else {
		ST_LOG_WARN("The list of parents was empty - Return nullptr");
		return nullptr;
	}
}

/*void NodeChild::setPos(glm::vec3 _pos, bool _convertToRelative){
glm::vec4 newPos(_pos, 1);
if(_convertToRelative){
newPos = getInverseModelMatrixHierarchical() * newPos;
}
parent->translate(glm::vec3(newPos.x, newPos.y, newPos.z), false);
}*/

/*glm::mat4 NodeChild::getInverseModelMatrixHierarchical(){
Transform * p = parent;
std::vector<glm::mat4> modelMatrixStack;
while(p != nullptr){
modelMatrixStack.push_back(p->getModelMatrix());
p = p->parent;
}

glm::mat4 modelMatrix(1);
for(signed long int i = modelMatrixStack.size()-1; i > 0; --i){
modelMatrix = modelMatrix * modelMatrixStack.at(i);
}
return glm::inverse(modelMatrix);
}*/

void NodeChild::printHierarchy(unsigned long int _startDepth, bool _last, std::vector<unsigned long int> & _p){
	for(unsigned long int j = 1; j <= _startDepth; ++j){
		if(j == _startDepth){
			std::cout << (_last ? char(0xC0) : char(0xC3)) << char(0xC4);
		}else{
			bool p = false;
			for(unsigned long int i : _p){
				if(j == i){
					p = true;
					break;
				}
			}
			if(p){
				std::cout << char(0xB3) << " ";
			}else{
				std::cout << "  ";
			}
		}
	}
	std::cout << typeid(*this).name() << " " << this << std::endl;
}

unsigned long int NodeChild::calculateDepth(unsigned long int _parent){
	unsigned long int depth = 0;
	std::vector<Transform * const> * p = &parents;
	if(p->size() > _parent){
		p = &p->at(_parent)->parents;
		depth += 1;
		while(p->size() > 0){
			p = &p->at(0)->parents;
			depth += 1;
		}
	}

	return depth;
}