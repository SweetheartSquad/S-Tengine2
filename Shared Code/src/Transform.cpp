#include "Transform.h"

Transform::Transform(void):
	Node(),
	translationVector(0.0f, 0.0f, 0.0f),
	scaleVector(1.f, 1.f, 1.f),
	orientation(1.0f, 0.0f, 0.0f, 0.0f){
	this->parent = nullptr;
}

Transform::~Transform(void){
}

void Transform::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	for(Transform * child : children){
		child->draw(projectionMatrix, viewMatrix);
	}
}

void Transform::update(){
}

void Transform::scale(float scaleX, float scaleY, float scaleZ){
	scaleVector.x *= scaleX;
	scaleVector.y *= scaleY;
	scaleVector.z *= scaleZ;
}

void Transform::scale(glm::vec3 scale)
{
	scaleVector *= scale;
}

void Transform::scaleX(float scaleX)
{
	scaleVector.x *= scaleX;
}

void Transform::scaleY(float scaleY)
{
	scaleVector.y *= scaleY;
}

void Transform::scaleZ(float scaleZ)
{
	scaleVector.z *= scaleZ;
}

void Transform::translate(float translateX, float translateY, float translateZ)
{
	translationVector.x += translateX;
	translationVector.y += translateY;
	translationVector.z += translateZ;
}

void Transform::translate(glm::vec3 translate)
{
	translationVector += translate;
}

void Transform::translateX(float translateX)
{
	translationVector.x += translateX;
}

void Transform::translateY(float translateY)
{
	translationVector.y += translateY;
}

void Transform::translateZ(float translateZ)
{
	translationVector.z += translateZ;
}

void Transform::rotate(glm::quat rotation){
	orientation = rotation * orientation;
}

void Transform::rotate(float angle, float x, float y, float z){
	this->rotate(glm::quat(glm::angleAxis(angle, glm::vec3(x,y,z))));
}

glm::mat4 Transform::getTranslationMatrix(){
	return glm::translate(translationVector);
}

glm::mat4 Transform::getScaleMatrix(){
	return glm::scale(scaleVector);
}

glm::mat4 Transform::getRotationMatrix(){
	return glm::toMat4(orientation);
}

glm::mat4 Transform::getModelMatrix(){
	return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();	
}

void Transform::addChild(Transform* child){
	child->setParent(this);
	children.push_back(child);
}

void Transform::removeChildAtIndex(int index){
	children.erase(children.begin()+index-1);
}

void Transform::setParent(Transform* parent){
	this->parent = parent;
}
