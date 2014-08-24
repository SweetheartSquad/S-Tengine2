#include "Transformable.h"

Transformable::Transformable(void){
	this->translationVector = new glm::vec3(0.0f,0.0f,0.0f);
	this->scaleVector = new glm::vec3(1.f,1.f,1.f);
	this->orientation = new glm::quat(1,0,0,0);
	this->children = new std::vector<Transformable*>();
	this->vertices = new std::vector<Vertex>();
	this->parent = nullptr;
}

Transformable::~Transformable(void)
{
}

void Transformable::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{		
	for(int i = 0; i<children->size(); i++)
	{
		children->at(i)->draw(projectionMatrix, viewMatrix);
	}
}

void Transformable::update()
{
}

void Transformable::scale(float scaleX, float scaleY, float scaleZ)
{
	scaleVector->x *= scaleX;
	scaleVector->y *= scaleY;
	scaleVector->z *= scaleZ;
}

void Transformable::scale(glm::vec3 scale)
{
	*scaleVector *= scale;
}

void Transformable::scaleX(float scaleX)
{
	scaleVector->x *= scaleX;
}

void Transformable::scaleY(float scaleY)
{
	scaleVector->y *= scaleY;
}

void Transformable::scaleZ(float scaleZ)
{
	scaleVector->z *= scaleZ;
}

void Transformable::translate(float translateX, float translateY, float translateZ)
{
	translationVector->x += translateX;
	translationVector->y += translateY;
	translationVector->z += translateZ;
}

void Transformable::translate(glm::vec3 translate)
{
	*translationVector += translate;
}

void Transformable::translateX(float translateX)
{
	translationVector->x += translateX;
}

void Transformable::translateY(float translateY)
{
	translationVector->y += translateY;
}

void Transformable::translateZ(float translateZ)
{
	translationVector->z += translateZ;
}

void Transformable::rotate(glm::quat rotation){
	*this->orientation = rotation * *this->orientation;
}

void Transformable::rotate(float angle, float x, float y, float z){
	this->rotate(glm::quat(glm::angleAxis(angle, glm::vec3(x,y,z))));
}

glm::mat4 Transformable::getTranslationMatrix()
{
	return glm::translate(*translationVector);
}

glm::mat4 Transformable::getScaleMatrix()
{
	return glm::scale(*scaleVector);
}

glm::mat4 Transformable::getRotationMatrix()
{
	return glm::toMat4(*orientation);
}

glm::mat4 Transformable::getModelMatrix()
{
	if(parent)
	{   
		return parent->getModelMatrix() * (getTranslationMatrix() * getRotationMatrix() * getScaleMatrix());
	}else{
		return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();	
	}
}

void Transformable::addChild(Transformable* child)
{
	child->setParent(this);
	children->push_back(child);
}

void Transformable::removeChildAtIndex(int index)
{
	children->erase(children->begin()+index-1);
}

void Transformable::setParent(Transformable* parent)
{
	this->parent = parent;
}

void Transformable::pushVert(Vertex vertex)
{
	vertices->push_back(vertex);
}