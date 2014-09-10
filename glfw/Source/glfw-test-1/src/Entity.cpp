#include "Entity.h"

Entity::Entity(MeshInterface * _mesh, Transform * _transform, ShaderInterface * _shader, Entity * _parent):
	mesh(_mesh),
	transform(_transform),
	shader(_shader),
	parent(_parent)
{
}

Entity::~Entity(void){
	delete transform;
	delete mesh;

	transform = nullptr;
	mesh = nullptr;
	shader = nullptr;
}

void Entity::draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix){
	//push transform
	vox::pushMatrix();
	vox::applyMatrix(transform->getModelMatrix());

	mesh->load();
	mesh->clean();
	mesh->render(shader, _projectionMatrix, _viewMatrix);

	for(Entity * child : children){
		child->draw(_projectionMatrix, _viewMatrix);
	}

	//pop transform
	vox::popMatrix();
}

void Entity::update(){
}

void Entity::addChild(Entity* _child){
	_child->setParent(this);
	children.push_back(_child);

	_child->transform->setParent(this->transform);
	transform->children.push_back(_child->transform);
}

void Entity::removeChildAtIndex(int _index){
	children.erase(children.begin()+_index-1);
	transform->children.erase(transform->children.begin()+_index-1);
}

void Entity::setParent(Entity* _parent){
	this->parent = _parent;
	transform->setParent(_parent->transform);
}

void Entity::setShader(ShaderInterface* _shader, bool _confiugreDefaultAttributes){
	shader = _shader;
	if(_confiugreDefaultAttributes){
		if(mesh != nullptr){
			mesh->configureDefaultVertexAttributes(shader);
		}
	}
}

void Entity::unload(){
	for(Entity * e : children){
		e->unload();
	}

	mesh->unload();
}

void Entity::reset(){
	for(Entity * e : children){
		e->reset();
	}

	mesh->load();
	mesh->clean();

	mesh->configureVertexAttributes(shader->get_aVertexPosition(), 3, 0);
	mesh->configureVertexAttributes(shader->get_aVertexColor(), 4, sizeof(float)*3);
	mesh->configureVertexAttributes(shader->get_aVertexNormals(), 3, sizeof(float)*7);

	std::string shaderVertSrc = shader->vertName;
	std::string shaderFragSrc = shader->fragName;

	delete shader;
	shader = new ShaderInterface(shaderVertSrc, shaderFragSrc);
}