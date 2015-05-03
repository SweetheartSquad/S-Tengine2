#include "Plane.h"
#include "MeshInterface.h"
#include "MeshFactory.h"

Plane::Plane(glm::vec3 _center, float _size) :
	NodeTransformable(new Transform()),
	MeshEntity(MeshFactory::getPlaneMesh(), transform)
{
	transform->scale(_size, _size, _size);
	transform->translate(_center);
}

Plane::~Plane(){
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