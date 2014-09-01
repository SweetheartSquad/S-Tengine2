#include "Entity.h"

Entity::Entity(void):Node(){
	this->children = new std::vector<Entity*>();
	this->vertices = new std::vector<Vertex>();
	this->indices = new std::vector<GLubyte>();
	this->parent = nullptr;
	this->transform = new Transform();
}

Entity::~Entity(void){
	delete children;
	delete vertices;
	delete indices;
	delete parent;
	delete transform;
}

void Entity::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){	
	
	vox::pushMatrix(transform->getModelMatrix() );
			GLUtils::checkForError(0,__FILE__,__LINE__);

	for(int i = 0; i<children->size(); i++){
		children->at(i)->draw(projectionMatrix, viewMatrix);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexInterface->getVertexInterfaceId());
	glUseProgram(shader->getProgramId());
	glm::mat4 mvp = projectionMatrix * viewMatrix * vox::modelMatrix();  	
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), vertices->data(), GL_STATIC_DRAW);	
	GLuint mvpUniformLocation = glGetUniformLocation(shader->getProgramId(), "MVP");
	glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	vertexInterface->renderVertexInterface();
	
	/*glBindBuffer(GL_ARRAY_BUFFER, vertexInterface->getVertexInterfaceId());
			GLUtils::checkForError(0,__FILE__,__LINE__);
	glUseProgram(shader->getProgramId());
			GLUtils::checkForError(0,__FILE__,__LINE__);
	glm::mat4 mvp = projectionMatrix * viewMatrix * vox::modelMatrix();  	
			GLUtils::checkForError(0,__FILE__,__LINE__);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices->size(), vertices->data(), GL_STATIC_DRAW);
			GLUtils::checkForError(0,__FILE__,__LINE__);
	GLuint mvpUniformLocation = glGetUniformLocation(shader->getProgramId(), "MVP");
			GLUtils::checkForError(0,__FILE__,__LINE__);
	glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	vertexInterface->renderVertexInterface();
		GLUtils::checkForError(0,__FILE__,__LINE__);*/
	
	
	// Render the Buffers
	
	/*glBindVertexArray(vertexInterface->vaoId);
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glDrawRangeElements(vertexInterface->polygonalDrawMode, 0, vertexInterface->vertCount, indices->size(), GL_UNSIGNED_BYTE, indices->data());
		GLUtils::checkForError(0,__FILE__,__LINE__);
	glBindVertexArray(0);
		GLUtils::checkForError(0,__FILE__,__LINE__);*/

	vox::popMatrix();
}

void Entity::update(){
}

void Entity::addChild(Entity* child)
{
	child->setParent(this);
	children->push_back(child);

	child->transform->setParent(this->transform);
	transform->children->push_back(child->transform);
}

void Entity::removeChildAtIndex(int index){
	children->erase(children->begin()+index-1);
	transform->children->erase(transform->children->begin()+index-1);
}

void Entity::setParent(Entity* parent){
	this->parent = parent;
	transform->setParent(parent->transform);
}

void Entity::pushVert(Vertex vertex){
	vertices->push_back(vertex);
}
void Entity::pushQuad(GLubyte v0, GLubyte v1, GLubyte v2, GLubyte v3){
	indices->push_back(v0);
	indices->push_back(v1);
	indices->push_back(v2);
	indices->push_back(v3);
}
void Entity::setNormal(unsigned long int _vertId, float _x, float _y, float _z){
	vertices->at(_vertId).nx = _x;
	vertices->at(_vertId).ny = _y;
	vertices->at(_vertId).nz = _z;
}