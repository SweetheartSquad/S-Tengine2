#include "Entity.h"


Entity::Entity(MeshInterface * _mesh, Transform * _transform, ShaderInterface * _shader, Entity * _parent):Node(){
	mesh = _mesh;
	transform = _transform;
	shader = _shader;
	parent = _parent;
}

Entity::~Entity(void){
	delete transform;
	delete mesh;

	transform = nullptr;
	mesh = nullptr;
	shader = nullptr;
}



void Entity::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	//push transform
	vox::pushMatrix();
	vox::applyMatrix(transform->getModelMatrix());

	mesh->load();
	mesh->clean();
	mesh->render(shader, projectionMatrix, viewMatrix);

	for(Entity * child : children){
		child->draw(projectionMatrix, viewMatrix);
	}

	//pop transform
	vox::popMatrix();
}

void Entity::update(){
}

void Entity::addChild(Entity* child){
	child->setParent(this);
	children.push_back(child);

	child->transform->setParent(this->transform);
	transform->children.push_back(child->transform);
}

void Entity::removeChildAtIndex(int index){
	children.erase(children.begin()+index-1);
	transform->children.erase(transform->children.begin()+index-1);
}

void Entity::setParent(Entity* parent){
	this->parent = parent;
	transform->setParent(parent->transform);
}
