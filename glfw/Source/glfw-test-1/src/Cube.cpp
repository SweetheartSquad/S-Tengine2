#include "Cube.h"

Cube::Cube(glm::vec3 center, float size):
	Entity(new QuadMesh(GL_QUADS, GL_STATIC_DRAW), new Transform())
{
	QuadMesh * m = (QuadMesh *)mesh;

	transform->scale(size, size, size);
	transform->translate(center);
	
	//Top
	m->pushVert(Vertex(-1.f, 1.f, 1.f));
	m->pushVert(Vertex(-1.f, 1.f, -1.f));
	m->pushVert(Vertex(1.f, 1.f, -1.f));
	m->pushVert(Vertex(1.f, 1.f, 1.f));
	m->pushQuad(0,1,2,3);
	m->setNormal(0, 0.0, 1.0, 0.0);
	m->setNormal(1, 0.0, 1.0, 0.0);
	m->setNormal(2, 0.0, 1.0, 0.0);
	m->setNormal(3, 0.0, 1.0, 0.0);
	
	//Bottom
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushVert(Vertex(1.f, -1.f, 1.f));
	m->pushVert(Vertex(1.f, -1.f, -1.f));
	m->pushVert(Vertex(-1.f, -1.f, -1.f));
	m->pushQuad(4,5,6,7);
	m->setNormal(4, 0.0, -1.0, 0.0);
	m->setNormal(5, 0.0, -1.0, 0.0);
	m->setNormal(6, 0.0, -1.0, 0.0);
	m->setNormal(7, 0.0, -1.0, 0.0);

	//Front
	m->pushVert(Vertex(-1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, 1.f, 1.f));
	m->pushVert(Vertex(1.f, -1.f, 1.f));
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushQuad(8,9,10,11);
	m->setNormal(8, 1.0, 0.0, 0.0);
	m->setNormal(9, 1.0, 0.0, 0.0);
	m->setNormal(10, 1.0, 0.0, 0.0);
	m->setNormal(11, 1.0, 0.0, 0.0);

	//Back
	m->pushVert(Vertex(-1.f, 1.f, -1.f));
	m->pushVert(Vertex(1.f, 1.f, -1.f));
	m->pushVert(Vertex(1.f, -1.f, -1.f));
	m->pushVert(Vertex(-1.f, -1.f, -1.f));
	m->pushQuad(15,14,13,12);
	m->setNormal(12, -1.0, 0.0, 0.0);
	m->setNormal(13, -1.0, 0.0, 0.0);
	m->setNormal(14, -1.0, 0.0, 0.0);
	m->setNormal(15, -1.0, 0.0, 0.0);

	//Left
	m->pushVert(Vertex(-1.f, 1.f, 1.f));
	m->pushVert(Vertex(-1.f, 1.f, -1.f));
	m->pushVert(Vertex(-1.f, -1.f, -1.f));
	m->pushVert(Vertex(-1.f, -1.f, 1.f));
	m->pushQuad(19,18,17,16);
	m->setNormal(16, 0.0, 0.0, 1.0);
	m->setNormal(17, 0.0, 0.0, 1.0);
	m->setNormal(18, 0.0, 0.0, 1.0);
	m->setNormal(19, 0.0, 0.0, 1.0);

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
}

Cube::~Cube(void){
}

void Cube::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	Entity::draw(projectionMatrix, viewMatrix);
}

void Cube::setColour(float red, float green, float blue, float alpha){
	for(int i=0; i<24; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;	
	}
}

void Cube::setLeftColour(float red, float green, float blue, float alpha){
	for(int i=16; i<20; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;	
	}
}

void Cube::setRightColour(float red, float green, float blue, float alpha){
	for(int i=20; i<24; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;	
	}
}

void Cube::setTopColour(float red, float green, float blue, float alpha){
	for(int i=0; i<4; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;	
	}
}

void Cube::setBottomColour(float red, float green, float blue, float alpha){
	for(int i=0; i<8; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;	
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
