#include "Entity.h"

Entity::Entity(void){
	x = 0;
	y = 0;
	z = 0;
	this->translationVector = new glm::vec3(0.0f,0.0f,0.0f);
	this->scaleVector = new glm::vec3(1.f,1.f,1.f);
	this->rotationVector = new glm::quat();
	this->children = new std::vector<Entity*>();
	this->vertices = new std::vector<Vertex>();
}

Entity::~Entity(void)
{
}

void Entity::draw()
{	

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), vertices->data(), GL_STATIC_DRAW);

	//glTranslatef(translationVector->x, translationVector->y, translationVector->z);
	//gmtl::AxisAnglef r;
	//gmtl::set(r, *rotationVector);
	glm::quat rotationT = glm::angleAxis(rotationVector->w, glm::vec3(rotationVector->x, rotationVector->y, rotationVector->z));
	//glRotatef(rotationT.w, rotationT.x, rotationT.y, rotationT.z);
	//glScalef(scaleVector->x, scaleVector->y, scaleVector->z);
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramId(), "MVP"), 1, GL_FALSE, &getModelViewMatrix()[0][0]);

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
	*this->rotationVector = rotation*(*this->rotationVector);
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
	return glm::rotate(rotationVector->w, glm::vec3(rotationVector->x, rotationVector->y, rotationVector->z));
}

glm::mat4 Entity::getModelViewMatrix()
{
	return glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f) * glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0)) * (getTranslationMatrix() * glm::mat4(1) * glm::mat4(1));
}

void Entity::rotate(float w, float x, float y, float z){
	glm::quat r;
	r = glm::rotate(r, w, glm::vec3(x,y,z));
	rotate(r);
}

void Entity::addChild(Entity* child)
{
	children->push_back(child);
}

void Entity::removeChildAtIndex(int index)
{
	children->erase(children->begin()+index-1);
}

void Entity::pushVert(Vertex vertex)
{
	vertices->push_back(vertex);
}