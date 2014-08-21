#include "Entity.h"

Entity::Entity(void){
	this->translationVector = new glm::vec3(0.0f,0.0f,0.0f);
	this->scaleVector = new glm::vec3(1.f,1.f,1.f);
	this->orientation = new glm::quat(1,0,0,0);
	this->children = new std::vector<Entity*>();
	this->vertices = new std::vector<Vertex>();
	this->parent = nullptr;
}

Entity::~Entity(void)
{
}

void Entity::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{		
	glUseProgram(shader->getProgramId());

	for(int i = 0; i<children->size(); i++)
	{
		children->at(i)->draw(projectionMatrix, viewMatrix);
	}

	glm::mat4 mvp = projectionMatrix * viewMatrix * getModelMatrix(); 
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), vertices->data(), GL_STATIC_DRAW);

	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramId(), "MVP"), 1, GL_FALSE, &mvp[0][0]);

	vertexBuffer->renderVertexBuffer();
}

void Entity::update()
{
}

void Entity::scale(float scaleX, float scaleY, float scaleZ)
{
	scaleVector->x *= scaleX;
	scaleVector->y *= scaleY;
	scaleVector->z *= scaleZ;
}

void Entity::scale(glm::vec3 scale)
{
	*scaleVector *= scale;
}

void Entity::scaleX(float scaleX)
{
	scaleVector->x *= scaleX;
}

void Entity::scaleY(float scaleY)
{
	scaleVector->y *= scaleY;
}

void Entity::scaleZ(float scaleZ)
{
	scaleVector->z *= scaleZ;
}

void Entity::translate(float translateX, float translateY, float translateZ)
{
	translationVector->x += translateX;
	translationVector->y += translateY;
	translationVector->z += translateZ;
}

void Entity::translate(glm::vec3 translate)
{
	*translationVector += translate.x;
}

void Entity::translateX(float translateX)
{
	translationVector->x += translateX;
}

void Entity::translateY(float translateY)
{
	translationVector->y += translateY;
}

void Entity::translateZ(float translateZ)
{
	translationVector->z += translateZ;
}

void Entity::rotate(glm::quat rotation){
	*this->orientation = rotation * *this->orientation;
}

void Entity::rotate(float angle, float x, float y, float z){
	this->rotate(glm::quat(glm::angleAxis(angle, glm::vec3(x,y,z))));
}

glm::mat4 Entity::getTranslationMatrix()
{
	return glm::translate(*translationVector);
}

glm::mat4 Entity::getScaleMatrix()
{
	return glm::scale(*scaleVector);
}

glm::mat4 Entity::getRotationMatrix()
{
	return glm::toMat4(*orientation);
}

glm::mat4 Entity::getModelMatrix()
{
	if(parent)
	{   
		*translationVector += *parent->translationVector;
		*scaleVector += *parent->scaleVector;

		glm::quat cpy = *orientation;

		*orientation *= *parent->orientation;

		glm::mat4 mat = (getTranslationMatrix() * getRotationMatrix() * getScaleMatrix()) * parent->getModelMatrix();

		*translationVector -= *parent->translationVector;
		*scaleVector -= *parent->scaleVector;
		*orientation = cpy;
		return mat;
	}else{
		return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();	
	}
}

void Entity::addChild(Entity* child)
{
	child->setParent(this);
	children->push_back(child);
}

void Entity::removeChildAtIndex(int index)
{
	children->erase(children->begin()+index-1);
}

void Entity::setParent(Entity* parent)
{
	this->parent = parent;
}

void Entity::pushVert(Vertex vertex)
{
	vertices->push_back(vertex);
}