#include "Cube.h"

Cube::Cube(glm::vec3 _center, float _size):
	Entity(new QuadMesh(GL_QUADS, GL_STATIC_DRAW), new Transform())
{
	QuadMesh * m = static_cast<QuadMesh *>(mesh);

	transform->scale(_size, _size, _size);
	transform->translate(_center);

	//Top
	m->pushVert(Vertex(-1.f, 1.f, 1.f)); //top left
	m->pushVert(Vertex(-1.f, 1.f, -1.f)); //bottom left
	m->pushVert(Vertex(1.f, 1.f, -1.f)); //bottom right
	m->pushVert(Vertex(1.f, 1.f, 1.f)); // top right
	m->pushQuad(0,1,2,3);
	m->setNormal(0, 0.0, 1.0, 0.0);
	m->setNormal(1, 0.0, 1.0, 0.0);
	m->setNormal(2, 0.0, 1.0, 0.0);
	m->setNormal(3, 0.0, 1.0, 0.0);
	m->setUV(0, 0.0, 0.0);
	m->setUV(1, 0.0, 1.0);
	m->setUV(2, 1.0, 1.0);
	m->setUV(3, 0.0, 1.0);

	//Bottom
	m->pushVert(Vertex(-1.f, -1.f, 1.f));//bottom left
	m->pushVert(Vertex(1.f, -1.f, 1.f));//top right
	m->pushVert(Vertex(1.f, -1.f, -1.f));//bottom right
	m->pushVert(Vertex(-1.f, -1.f, -1.f));//bottom left
	m->pushQuad(4,5,6,7);
	m->setNormal(4, 0.0, -1.0, 0.0);
	m->setNormal(5, 0.0, -1.0, 0.0);
	m->setNormal(6, 0.0, -1.0, 0.0);
	m->setNormal(7, 0.0, -1.0, 0.0);
	m->setUV(4, 0.0, 0.0);
	m->setUV(5, 0.0, 1.0);
	m->setUV(6, 1.0, 1.0);
	m->setUV(7, 0.0, 1.0);

	//Front
	m->pushVert(Vertex(-1.f, 1.f, 1.f));//top left
	m->pushVert(Vertex(1.f, 1.f, 1.f));//top right
	m->pushVert(Vertex(1.f, -1.f, 1.f));//bottom right
	m->pushVert(Vertex(-1.f, -1.f, 1.f));//bottom left
	m->pushQuad(8,9,10,11);
	m->setNormal(8, 1.0, 0.0, 0.0);
	m->setNormal(9, 1.0, 0.0, 0.0);
	m->setNormal(10, 1.0, 0.0, 0.0);
	m->setNormal(11, 1.0, 0.0, 0.0);
	m->setUV(8, 0.0, 0.0);
	m->setUV(9, 0.0, 1.0);
	m->setUV(10, 1.0, 1.0);
	m->setUV(11, 0.0, 1.0);

	//Back
	m->pushVert(Vertex(-1.f, 1.f, -1.f));//top left
	m->pushVert(Vertex(1.f, 1.f, -1.f));//top right
	m->pushVert(Vertex(1.f, -1.f, -1.f));//bottom right
	m->pushVert(Vertex(-1.f, -1.f, -1.f));//bottom left
	m->pushQuad(15,14,13,12);
	m->setNormal(12, -1.0, 0.0, 0.0);
	m->setNormal(13, -1.0, 0.0, 0.0);
	m->setNormal(14, -1.0, 0.0, 0.0);
	m->setNormal(15, -1.0, 0.0, 0.0);
	m->setUV(12, 0.0, 0.0);
	m->setUV(13, 0.0, 1.0);
	m->setUV(14, 1.0, 1.0);
	m->setUV(15, 0.0, 1.0);

	//Left
	m->pushVert(Vertex(-1.f, 1.f, 1.f));//top
	m->pushVert(Vertex(-1.f, 1.f, -1.f));//
	m->pushVert(Vertex(-1.f, -1.f, -1.f));//bottom left
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushQuad(19,18,17,16);
	m->setNormal(16, 0.0, 0.0, 1.0);
	m->setNormal(17, 0.0, 0.0, 1.0);
	m->setNormal(18, 0.0, 0.0, 1.0);
	m->setNormal(19, 0.0, 0.0, 1.0);
	m->setUV(16, 0.0, 0.0);
	m->setUV(17, 0.0, 1.0);
	m->setUV(18, 1.0, 1.0);
	m->setUV(19, 0.0, 1.0);

	//Right
	m->pushVert(Vertex(1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, 1.f, -1.f));
	m->pushVert(Vertex(1.f, -1.f, -1.f));
	m->pushVert(Vertex(1.f, -1.f, 1.f));
	m->pushQuad(20,21,22,23);
	m->setNormal(20, 0.0, 0.0, -1.0);
	m->setNormal(21, 0.0, 0.0, -1.0);
	m->setNormal(22, 0.0, 0.0, -1.0);
	m->setNormal(23, 0.0, 0.0, -1.0);
	m->setUV(20, 0.0, 0.0);
	m->setUV(21, 0.0, 1.0);
	m->setUV(22, 1.0, 1.0);
	m->setUV(23, 0.0, 1.0);
}

Cube::~Cube(void){
}

void Cube::draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix){
	Entity::draw(_projectionMatrix, _viewMatrix);
}

void Cube::setColour(float red, float green, float blue, float alpha){
	for(int i=0; i<24; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;
	}
}

void Cube::setLeftColour(float _red, float _green, float _blue, float _alpha){
	for(int i=16; i<20; i++){
		mesh->vertices.at(i).red = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Cube::setRightColour(float _red, float _green, float _blue, float _alpha){
	for(int i=20; i<24; i++){
		mesh->vertices.at(i).red = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Cube::setTopColour(float _red, float _green, float _blue, float _alpha){
	for(int i=0; i<4; i++){
		mesh->vertices.at(i).red = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Cube::setBottomColour(float _red, float _green, float _blue, float _alpha){
	for(int i=0; i<8; i++){
		mesh->vertices.at(i).red = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Cube::setFrontColour(float red, float green, float blue, float alpha){
	for(int i=8; i<12; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;
	}
}

void Cube::setBackColour(float red, float green, float blue, float alpha){
	for(int i=12; i<16; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;
	}
}

void Cube::setVertexColour(int index, float red, float green, float blue, float alpha){
	mesh->vertices.at(index).red = red;
	mesh->vertices.at(index).green = green;
	mesh->vertices.at(index).blue = blue;
	mesh->vertices.at(index).alpha = alpha;
}

void Cube::update(void){
}