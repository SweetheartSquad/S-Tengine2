#pragma once

#include <node\NodeChild.h>
#include <Transform.h>

NodeChild::NodeChild(Transform * _parent):
	parent(_parent),
	transformDirty(true)
{
}

void NodeChild::makeDirty(){
	transformDirty = true;
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
	if(parent == nullptr){
		return glm::vec3(0,0,0);
	}
	if(transformDirty){
		glm::vec4 res(parent->getTranslationVector(), 1);
		Transform * p = parent;
		std::vector<glm::mat4> modelMatrixStack;
		while (p != nullptr){
			modelMatrixStack.push_back(p->getModelMatrix());
			p = p->parent;
		}
		
		glm::mat4 modelMatrix(1);
		for(signed long int i = modelMatrixStack.size()-1; i > 0; --i){
			modelMatrix = modelMatrix * modelMatrixStack.at(i);
		}
		res = modelMatrix * res;
		worldPos = glm::vec3(res);
		transformDirty = false;
	}
	return worldPos;
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