#include "Entity.h"

Entity::Entity(void):Node(){
	vertexInterface = nullptr;
	parent = nullptr;
	children = new std::vector<Entity *>();
	transform = new Transform();
}

Entity::~Entity(void){
	delete children;
	delete parent;
	delete transform;
	delete vertexInterface;

	children = nullptr;
	parent = nullptr;
	transform = nullptr;
	vertexInterface = nullptr;
}



void Entity::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	//push transform
	vox::pushMatrix();
	vox::applyMatrix(transform->getModelMatrix());

	vertexInterface->load();
	vertexInterface->clean();
	vertexInterface->render(shader, projectionMatrix, viewMatrix);

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
	vertexInterface->vertices->push_back(vertex);

	vertexInterface->dirty = true;
}
void Entity::pushTri(GLubyte v0, GLubyte v1, GLubyte v2){
	vertexInterface->indices->push_back(v0);
	vertexInterface->indices->push_back(v1);
	vertexInterface->indices->push_back(v2);

	vertexInterface->dirty = true;
}
void Entity::pushQuad(GLubyte v0, GLubyte v1, GLubyte v2, GLubyte v3){
	vertexInterface->indices->push_back(v0);
	vertexInterface->indices->push_back(v1);
	vertexInterface->indices->push_back(v2);
	vertexInterface->indices->push_back(v3);

	vertexInterface->dirty = true;
}
void Entity::setNormal(unsigned long int _vertId, float _x, float _y, float _z){
	vertexInterface->vertices->at(_vertId).nx = _x;
	vertexInterface->vertices->at(_vertId).ny = _y;
	vertexInterface->vertices->at(_vertId).nz = _z;

	vertexInterface->dirty = true;
}