#pragma once

#include <node\NodeChild.h>
#include <Transform.h>

NodeChild::NodeChild(Transform * _parent):
	parent(_parent),
	cumulativeModelMatrixDirty(true),
	cumulativeModelMatrix(glm::mat4(1))
{
}

void NodeChild::makeCumulativeModelMatrixDirty(){
	cumulativeModelMatrixDirty = true;
}

bool NodeChild::hasAncestor(Transform * _parent){
	if(_parent == nullptr){
		return false;
	}
	Transform * p = parent;
	while(p != nullptr){
		if(p == _parent){
			return true;
		}
		p = p->parent;
	}

	return false;
}

glm::vec3 NodeChild::getWorldPos(){
	// if the node has no parent, return a zero-vector
	if(parent == nullptr){
		return glm::vec3(0);
	}

	// if the cumulative model matrix is out-of-date, then so is the stored world position
	if(cumulativeModelMatrixDirty){
		// find the first non-zero ancestor translation vector
		glm::vec3 res(0);
		Transform * p = parent;
		if(p != nullptr){
			do{
				res = p->getTranslationVector();
				p = p->parent;
			}while(res == glm::vec3(0) && p != nullptr);

			// apply the cumulative matrix of its parent to the vector
			if(p != nullptr){
				res = glm::vec3(p->getCumulativeModelMatrix() * glm::vec4(res, 1));
			}
		}

		worldPos = res;
		cumulativeModelMatrixDirty = false;
	}
	return worldPos;
}

void NodeChild::setParent(Transform * _parent){
	if(_parent != parent){
		this->parent = _parent;
		makeCumulativeModelMatrixDirty();
	}
}

void NodeChild::setPos(glm::vec3 _pos, bool _convertToRelative){
	glm::vec4 newPos(_pos, 1);
	if(_convertToRelative){
		newPos = getInverseModelMatrixHierarchical() * newPos;
	}
	parent->translate(glm::vec3(newPos.x, newPos.y, newPos.z), false);
}

glm::mat4 NodeChild::getInverseModelMatrixHierarchical(){
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
}

void NodeChild::printHierarchy(unsigned long int _startDepth){
	for(unsigned long int j = 1; j <= _startDepth; ++j){
		if(j == _startDepth){
			std::cout << char(192) << char(196);
		}else{
			std::cout << "  ";
		}
	}
	std::cout << this << std::endl;
}