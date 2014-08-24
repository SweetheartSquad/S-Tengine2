#include "Entity.h"

Entity::Entity(void){
	this->children = new std::vector<Entity*>();
	this->vertices = new std::vector<Vertex>();
	this->parent = nullptr;
	this->transform = new Transform();
}

Entity::~Entity(void)
{
}

void Entity::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{	
	for(int i = 0; i<children->size(); i++)
	{
		children->at(i)->draw(projectionMatrix, viewMatrix);
	}

	glm::mat4 mvp = projectionMatrix * viewMatrix * transform->getModelMatrix(); 

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), vertices->data(), GL_STATIC_DRAW);

	glUniformMatrix4fv(glGetUniformLocation(shader->getProgramId(), "MVP"), 1, GL_FALSE, &mvp[0][0]);

	glUseProgram(shader->getProgramId());

	vertexBuffer->renderVertexBuffer();
}

void Entity::update()
{
}

void Entity::addChild(Entity* child)
{
	child->setParent(this);
	children->push_back(child);

	child->transform->setParent(this->transform);
	transform->children->push_back(child->transform);
}

void Entity::removeChildAtIndex(int index)
{
	children->erase(children->begin()+index-1);
	transform->children->erase(transform->children->begin()+index-1);
}

void Entity::setParent(Entity* parent)
{
	this->parent = parent;
	transform->setParent(parent->transform);
}

void Entity::pushVert(Vertex vertex)
{
	vertices->push_back(vertex);
}