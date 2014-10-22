#include "Plane.h"

Plane::Plane(glm::vec3 _center, float _size)
	:Entity(new QuadMesh(GL_QUADS, GL_STATIC_DRAW), new Transform())
{
	QuadMesh * m = static_cast<QuadMesh *>(mesh);
	
	transform->scale(_size, _size, _size);
	transform->translate(_center);

	m->pushVert(Vertex(-1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, -1.f, 1.f));
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushQuad(8, 9, 10, 11);
	m->setNormal(8, 0.0, 0.0, 1.0);
	m->setNormal(9, 0.0, 0.0, 1.0);
	m->setNormal(10, 0.0, 0.0, 1.0);
	m->setNormal(11, 0.0, 0.0, 1.0);
	m->setUV(8, 0.0, 0.0);
	m->setUV(9, 0.0, 1.0);
	m->setUV(10, 1.0, 1.0);
	m->setUV(11, 1.0, 0.0);
}

Plane::~Plane(){
}

void Plane::draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, std::vector<Light*> _lights){
	Entity::draw(_projectionMatrix, _viewMatrix, _lights);
}

void Plane::update(){
	Entity::update();
}

void Plane::setColour(float _red, float _green, float _blue, float _alpha){
	for(int i=0; i<4; i++){
		mesh->vertices.at(i).red   = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue  = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Plane::setVertexColour(int _index, float _red, float _green, float _blue, float _alpha){
	mesh->vertices.at(_index).red   = _red;
	mesh->vertices.at(_index).green = _green;
	mesh->vertices.at(_index).blue  = _blue;
	mesh->vertices.at(_index).alpha = _alpha;
}