#pragma once

#include "Transform.h"

Transform::Transform():
	translationVector(0.f, 0.f, 0.f),
	scaleVector(1.f, 1.f, 1.f),
	orientation(1.f, 0.f, 0.f, 0.f)
{
}

Transform::~Transform(){
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
}

void Transform::rotate(glm::quat _rotation, CoordinateSpace _space){
	switch(_space){
	case kWORLD:
		orientation = _rotation * orientation;
		break;
	case kOBJECT:
		orientation = orientation * _rotation;
		break;
	}
}

void Transform::rotate(float _angle, float _x, float _y, float _z, CoordinateSpace _space){
	this->rotate(glm::quat(glm::angleAxis(_angle, glm::vec3(_x, _y, _z))), _space);
}

glm::mat4 Transform::getTranslationMatrix(){
	return glm::translate(translationVector);
}

glm::mat4 Transform::getScaleMatrix(){
	return glm::scale(scaleVector);
}

glm::mat4 Transform::getOrientationMatrix(){
	return glm::toMat4(orientation);
}

glm::mat4 Transform::getModelMatrix(){
	return getTranslationMatrix() * getOrientationMatrix() * getScaleMatrix();
}

void Transform::reset(){
	translationVector = glm::vec3(0.f, 0.f, 0.f);
	scaleVector = glm::vec3(1.f, 1.f, 1.f);
	orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
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