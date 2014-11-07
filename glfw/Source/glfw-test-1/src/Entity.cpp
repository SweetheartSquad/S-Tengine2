#include "Entity.h"

Entity::Entity(MeshInterface * _mesh, Transform * _transform, Shader * _shader, Entity * _parent):
	mesh(_mesh),
	NodeAnimatable(_transform),
	shader(_shader),
	NodeHierarchical(_parent)
{
	if(mesh != nullptr && shader != nullptr){
		reset();
	}
}

Entity::~Entity(void){
	delete transform;
	delete mesh;
	shader->decrementAndDelete();
	transform = nullptr;
	mesh = nullptr;
	shader = nullptr;
}

void Entity::draw(MatrixStack * _matrixStack, RenderStack * _renderStack){
	//push transform
	_matrixStack->pushMatrix();
	_matrixStack->applyMatrix(transform->getModelMatrix());

	mesh->load();
	mesh->clean();
	_renderStack->shader = shader;
	mesh->render(_matrixStack, _renderStack);

	for(Node * child : children){
		dynamic_cast<Entity *>(child)->draw(_matrixStack, _renderStack);
	}
	//pop transform
	_matrixStack->popMatrix();
}

void Entity::update(){
}

void Entity::addChild(Entity * _child){
	_child->setParent(this);
	children.push_back(_child);

	_child->transform->setParent(this->transform);
	transform->children.push_back(_child->transform);
}

void Entity::removeChildAtIndex(int _index){
	children.erase(children.begin()+_index-1);
	transform->children.erase(transform->children.begin()+_index-1);
}

void Entity::setParent(Entity * _parent){
	this->parent = _parent;
	transform->setParent(_parent->transform);
}

void Entity::setShader(Shader * _shader, bool _confiugreDefaultAttributes){
	shader = _shader;
	if(_confiugreDefaultAttributes){
		if(mesh != nullptr){
			mesh->configureDefaultVertexAttributes(shader);
		}
	}
}

void Entity::unload(){
	for(Node * child : children){
		dynamic_cast<Entity *>(child)->unload();
	}

	mesh->unload();
	shader->unload();
}

void Entity::reset(){
	for(Node * child : children){
		dynamic_cast<Entity *>(child)->reset();
	}

	mesh->load();
	mesh->clean();

	shader->load();

	mesh->configureDefaultVertexAttributes(shader);
}