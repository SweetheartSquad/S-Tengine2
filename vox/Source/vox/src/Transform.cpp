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

void Transform::scale(float _scaleX, float _scaleY, float _scaleZ){
	scaleVector.x *= _scaleX;
	scaleVector.y *= _scaleY;
	scaleVector.z *= _scaleZ;
}

void Transform::scale(glm::vec3 _scale){
	scaleVector *= _scale;
}

void Transform::scaleX(float _scaleX){
	scaleVector.x *= _scaleX;
}

void Transform::scaleY(float _scaleY){
	scaleVector.y *= _scaleY;
}

void Transform::scaleZ(float _scaleZ){
	scaleVector.z *= _scaleZ;
}

void Transform::translate(float _translateX, float _translateY, float _translateZ){
	translationVector.x += _translateX;
	translationVector.y += _translateY;
	translationVector.z += _translateZ;
}

void Transform::translate(glm::vec3 _translate){
	translationVector += _translate;
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