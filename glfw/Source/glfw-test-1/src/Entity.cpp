#include "Entity.h"

Entity::Entity(void):Node(){
	this->children = new std::vector<Entity*>();
	this->vertices = new std::vector<Vertex>();
	this->indices = new std::vector<GLubyte>();
	this->parent = nullptr;
	this->transform = new Transform();

	dirty = false;
}

Entity::~Entity(void){
	delete children;
	delete vertices;
	delete indices;
	delete parent;
	delete transform;
}

void Entity::clean(){
	if(dirty){
		glBindBuffer(GL_ARRAY_BUFFER, vertexInterface->vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*(vertices->size()), vertices->data(), GL_STATIC_DRAW);
		GLUtils::checkForError(0,__FILE__,__LINE__);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexInterface->iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*(indices->size()), indices->data(), GL_STATIC_DRAW);
		GLUtils::checkForError(0,__FILE__,__LINE__);
		dirty = false;
	}
}
void Entity::render(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	
	std::cout << vertexInterface->vaoId << std::endl;
	//bind VAO
	glBindVertexArray(vertexInterface->vaoId);
	GLUtils::checkForError(0,__FILE__,__LINE__);

	//specify shader attributes
	glUseProgram(shader->getProgramId());
	GLUtils::checkForError(0,__FILE__,__LINE__);
	glm::mat4 mvp = projectionMatrix * viewMatrix * vox::modelMatrix();  	
	GLuint mvpUniformLocation = glGetUniformLocation(shader->getProgramId(), "MVP");
	glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);
	GLUtils::checkForError(0,__FILE__,__LINE__);

	//draw
	std::cout << indices->size() << std::endl;
	std::cout << vertices->size() << std::endl;
	glDrawRangeElements(vertexInterface->polygonalDrawMode, 0, vertices->size(), indices->size(), GL_UNSIGNED_BYTE, 0);
	GLUtils::checkForError(0,__FILE__,__LINE__);

	//disable VAO
	glBindVertexArray(0);
}

void Entity::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){	
	//push transform
	vox::pushMatrix(transform->getModelMatrix() );

	clean();
	render(projectionMatrix, viewMatrix);

	for(Entity * child : *children){
		child->draw(projectionMatrix, viewMatrix);
	}

	//pop transform
	vox::popMatrix();
}

void Entity::update(){
}

void Entity::addChild(Entity* child){
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

	dirty = true;
}
void Entity::pushTri(GLubyte v0, GLubyte v1, GLubyte v2){
	indices->push_back(v0);
	indices->push_back(v1);
	indices->push_back(v2);

	dirty = true;
}
void Entity::pushQuad(GLubyte v0, GLubyte v1, GLubyte v2, GLubyte v3){
	indices->push_back(v0);
	indices->push_back(v1);
	indices->push_back(v2);
	indices->push_back(v3);

	dirty = true;
}
void Entity::setNormal(unsigned long int _vertId, float _x, float _y, float _z){
	vertices->at(_vertId).nx = _x;
	vertices->at(_vertId).ny = _y;
	vertices->at(_vertId).nz = _z;

	dirty = true;
}