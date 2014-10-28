#pragma once

#include "Joint.h"
#include "Transform.h"

#include <cinder\app\App.h>

unsigned long int Joint::nextId = 0;
uint32_t Joint::nextColor = 0xFFFFFF;
std::map<uint32_t, Joint*> Joint::jointMap;

void Joint::deleteJoints(NodeHierarchical * _j){
	for(unsigned long int i = 0; i < _j->children.size(); ++i){
		deleteJoints(_j->children.at(i));
	}
	delete _j;
}

void Joint::init(){
	depth = 0;
	parent = nullptr;
	jointMap.insert(JointPair(nextColor, this));
	color = Color::hex(nextColor);
	nextColor -= 0x000001;
	id = nextId;
	nextId += 1;
}

Joint::Joint() : 
	NodeTransformable(new Transform()),
	NodeHierarchical(nullptr){
	init();
}

Joint::Joint(NodeHierarchical * _parent) : 
	NodeTransformable(new Transform()),
	NodeHierarchical(_parent)
{
	init();
	parent = _parent;
	while(_parent != nullptr){
		_parent = _parent->parent;
		depth += 1;
	}
}

void Joint::setPos(Vec3d _pos, bool _convertToRelative){
	glm::vec4 newPos(_pos.x, _pos.y, _pos.z, 1);
	if(_convertToRelative){
		NodeHierarchical * _parent = parent;
		std::vector<glm::mat4> modelMatrixStack;
		while(_parent != nullptr){
			modelMatrixStack.push_back(dynamic_cast<NodeTransformable *>(_parent)->transform->getModelMatrix());
			_parent = _parent->parent;
		}

		glm::mat4 modelMatrix(1);
		for(unsigned long int i = modelMatrixStack.size(); i > 0; --i){
			modelMatrix = modelMatrix * modelMatrixStack.at(i-1);
		}
		newPos = glm::inverse(modelMatrix) * newPos;
	}
	transform->translationVector = glm::vec3(newPos.x, newPos.y, newPos.z);
}

Vec3d Joint::getPos(bool _relative){
	glm::vec4 res(transform->translationVector.x, transform->translationVector.y, transform->translationVector.z, 1);
	if(!_relative){
		NodeHierarchical * _parent = parent;
		std::vector<glm::mat4> modelMatrixStack;
		while(_parent != nullptr){
			modelMatrixStack.push_back(dynamic_cast<NodeTransformable *>(_parent)->transform->getModelMatrix());
			_parent = _parent->parent;
		}
		
		glm::mat4 modelMatrix(1);
		for(unsigned long int i = modelMatrixStack.size(); i > 0; --i){
			modelMatrix = modelMatrix * modelMatrixStack.at(i-1);
		}
		res = modelMatrix * res;
	}
	return Vec3d(res.x, res.y, res.z);
}

Joint::~Joint(){
	delete transform;
}

void Joint::draw(gl::GlslProg * _shader){
	//gl::enableWireframe();
	_shader->uniform("pickingColor", color);
	//colour
	if(depth%3 == 0){
		gl::color(0, 1, 1);
	}else if(depth%3 == 1){
		gl::color(1, 0, 1);
	}else{
		gl::color(1, 1, 0);
	}
	
	//draw joint
	gl::pushModelView();
		gl::translate(transform->translationVector.x,
							transform->translationVector.y,
							transform->translationVector.z);

		//gl::pushMatrices(); 
			gl::rotate(Quatd(transform->orientation.w,
							 transform->orientation.x,
							 transform->orientation.y,
							 transform->orientation.z));
			gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.05f);
		//gl::popMatrices();

	//draw bones
	for(NodeHierarchical * child : children){
		Vec3d cinderTrans(
			dynamic_cast<NodeTransformable *>(child)->transform->translationVector.x,
			dynamic_cast<NodeTransformable *>(child)->transform->translationVector.y,
			dynamic_cast<NodeTransformable *>(child)->transform->translationVector.z
		);
		Quatd boneDir(Vec3d(0.0, 1.0, 0.0), cinderTrans);
		gl::pushMatrices();
			gl::rotate(boneDir);

			gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));
			gl::rotate(Vec3f(0.f, 90.f, 0.f));
			gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));
		gl::popMatrices();

		dynamic_cast<Joint *>(child)->draw(_shader);
	}
	gl::popModelView();

	//gl::disableWireframe();
}