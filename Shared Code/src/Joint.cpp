#pragma once

#include "Joint.h"

unsigned long int Joint::nextId = 0;
uint32_t Joint::nextColor = 0xFFFFFF;
std::map<uint32_t, Joint*> Joint::jointMap;

void Joint::deleteJoints(Joint * _j){
	for(unsigned long int i = 0; i < _j->children.size(); ++i){
		deleteJoints(_j->children.at(i));
	}
	delete _j;
}

void Joint::init(){
	depth = 0;
	parent = NULL;
	jointMap.insert(JointPair(nextColor, this));
	color = Color::hex(nextColor);
	nextColor -= 0x000001;
	id = nextId;
	nextId += 1;
}

Joint::Joint(){
	init();
}

Joint::Joint(Joint * _parent){
	init();
	parent = _parent;
	while(_parent != nullptr){
		_parent = _parent->parent;
		depth += 1;
	}
}

void Joint::setPos(Vec3d _pos, bool _convertToRelative){
	glm::vec3 glmPos = glm::vec3(_pos.x, _pos.y, _pos.z);
	if(_convertToRelative){
		Joint * _parent = parent;
		while(_parent != nullptr){
			glmPos -= _parent->transform.translationVector;
			_parent = _parent->parent;
		}
	}
	transform.translationVector = glmPos;
}
Vec3d Joint::getPos(bool _relative){
	glm::vec3 res = transform.translationVector;
	if(!_relative){
		Joint * _parent = parent;
		while(_parent != nullptr){
			res += _parent->transform.translationVector;
			_parent = _parent->parent;
		}
	}
	return Vec3d(res.x, res.y, res.z);
}

Joint::~Joint(){}

void Joint::draw(gl::GlslProg * _shader){
	//gl::enableWireframe();
	_shader->uniform("pickingColor", color);
	//colour
	if(depth%3 == 0){
		gl::color(Color(0, 1, 1));
	}else if(depth%3 == 1){
		gl::color(Color(1, 0, 1));
	}else{
		gl::color(Color(1, 1, 0));
	}
	
	//draw joint
	gl::pushModelView();
		gl::translate(transform.translationVector.x,
							transform.translationVector.y,
							transform.translationVector.z);

		gl::pushMatrices(); 
			gl::rotate(Quatd(transform.orientation.w,
							 transform.orientation.x,
							 transform.orientation.y,
							 transform.orientation.z));
			gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.05f);
		gl::popMatrices();

	//draw bones
	for(Joint * child : children){
		Vec3d cinderTrans(
			child->transform.translationVector.x,
			child->transform.translationVector.y,
			child->transform.translationVector.z
		);
		Quatd boneDir(Vec3d(0.0, 1.0, 0.0), cinderTrans);
		gl::pushMatrices();
			gl::rotate(boneDir);

			gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));
			gl::rotate(Vec3f(0.f, 90.f, 0.f));
			gl::drawSolidTriangle(Vec2f(0.05f, 0.f), Vec2f(-0.05f, 0.f), Vec2f(0.f, cinderTrans.length()));
		gl::popMatrices();

		child->draw(_shader);
	}
	gl::popModelView();

	//gl::disableWireframe();
}