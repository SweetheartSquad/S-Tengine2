#include "Cube.h"

Cube::Cube(glm::vec3 center, float size){
	vertexInterface = new VertexInterface(GL_QUADS, GL_STATIC_DRAW);
	
	transform->scale(size, size, size);
	transform->translate(center);
	
	//Top
	pushVert(Vertex(-1.f, 1.f, 1.f));
	pushVert(Vertex(-1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, 1.f, 1.f));
	pushQuad(0,1,2,3);
	setNormal(0, 0.0, 1.0, 0.0);
	setNormal(1, 0.0, 1.0, 0.0);
	setNormal(2, 0.0, 1.0, 0.0);
	setNormal(3, 0.0, 1.0, 0.0);
	
	//Bottom
	pushVert(Vertex(-1.f, -1.f, 1.f));
	pushVert(Vertex(1.f, -1.f, 1.f));
	pushVert(Vertex(1.f, -1.f, -1.f));
	pushVert(Vertex(-1.f, -1.f, -1.f));
	pushQuad(4,5,6,7);
	setNormal(4, 0.0, -1.0, 0.0);
	setNormal(5, 0.0, -1.0, 0.0);
	setNormal(6, 0.0, -1.0, 0.0);
	setNormal(7, 0.0, -1.0, 0.0);

	//Front
	pushVert(Vertex(-1.f, 1.f, 1.f));
	pushVert(Vertex(1.f, 1.f, 1.f));
	pushVert(Vertex(1.f, -1.f, 1.f));
	pushVert(Vertex(-1.f, -1.f, 1.f));
	pushQuad(8,9,10,11);
	setNormal(8, 1.0, 0.0, 0.0);
	setNormal(9, 1.0, 0.0, 0.0);
	setNormal(10, 1.0, 0.0, 0.0);
	setNormal(11, 1.0, 0.0, 0.0);

	//Back
	pushVert(Vertex(-1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, -1.f, -1.f));
	pushVert(Vertex(-1.f, -1.f, -1.f));
	pushQuad(15,14,13,12);
	setNormal(12, -1.0, 0.0, 0.0);
	setNormal(13, -1.0, 0.0, 0.0);
	setNormal(14, -1.0, 0.0, 0.0);
	setNormal(15, -1.0, 0.0, 0.0);

	//Left
	pushVert(Vertex(-1.f, 1.f, 1.f));
	pushVert(Vertex(-1.f, 1.f, -1.f));
	pushVert(Vertex(-1.f, -1.f, -1.f));
	pushVert(Vertex(-1.f, -1.f, 1.f));
	pushQuad(19,18,17,16);
	setNormal(16, 0.0, 0.0, 1.0);
	setNormal(17, 0.0, 0.0, 1.0);
	setNormal(18, 0.0, 0.0, 1.0);
	setNormal(19, 0.0, 0.0, 1.0);

	//Right
	pushVert(Vertex(1.f, 1.f, 1.f));
	pushVert(Vertex(1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, -1.f, -1.f));
	pushVert(Vertex(1.f, -1.f, 1.f));
	pushQuad(20,21,22,23);
	setNormal(20, 0.0, 0.0, -1.0);
	setNormal(21, 0.0, 0.0, -1.0);
	setNormal(22, 0.0, 0.0, -1.0);
	setNormal(23, 0.0, 0.0, -1.0);
	
}

Cube::~Cube(void){
}

void Cube::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	Entity::draw(projectionMatrix, viewMatrix);
}

void Cube::setColour(float red, float green, float blue, float alpha){
	for(int i=0; i<24; i++){
		vertexInterface->vertices->at(i).red = red;
		vertexInterface->vertices->at(i).green = green;
		vertexInterface->vertices->at(i).blue = blue;
		vertexInterface->vertices->at(i).alpha = alpha;	
	}
}

void Cube::setLeftColour(float red, float green, float blue, float alpha){

	for(int i=16; i<20; i++){
		vertexInterface->vertices->at(i).red = red;
		vertexInterface->vertices->at(i).green = green;
		vertexInterface->vertices->at(i).blue = blue;
		vertexInterface->vertices->at(i).alpha = alpha;	
	}

}

void Cube::setRightColour(float red, float green, float blue, float alpha){
	for(int i=20; i<24; i++){
		vertexInterface->vertices->at(i).red = red;
		vertexInterface->vertices->at(i).green = green;
		vertexInterface->vertices->at(i).blue = blue;
		vertexInterface->vertices->at(i).alpha = alpha;	
	}
}

void Cube::setTopColour(float red, float green, float blue, float alpha){
	for(int i=0; i<4; i++){
		vertexInterface->vertices->at(i).red = red;
		vertexInterface->vertices->at(i).green = green;
		vertexInterface->vertices->at(i).blue = blue;
		vertexInterface->vertices->at(i).alpha = alpha;	
	}
}

void Cube::setBottomColour(float red, float green, float blue, float alpha){
	for(int i=0; i<8; i++){
		vertexInterface->vertices->at(i).red = red;
		vertexInterface->vertices->at(i).green = green;
		vertexInterface->vertices->at(i).blue = blue;
		vertexInterface->vertices->at(i).alpha = alpha;	
	}
}

void Cube::setFrontColour(float red, float green, float blue, float alpha){
	for(int i=8; i<12; i++){
		vertexInterface->vertices->at(i).red = red;
		vertexInterface->vertices->at(i).green = green;
		vertexInterface->vertices->at(i).blue = blue;
		vertexInterface->vertices->at(i).alpha = alpha;	
	}
}

void Cube::setBackColour(float red, float green, float blue, float alpha){
	for(int i=12; i<16; i++){
		vertexInterface->vertices->at(i).red = red;
		vertexInterface->vertices->at(i).green = green;
		vertexInterface->vertices->at(i).blue = blue;
		vertexInterface->vertices->at(i).alpha = alpha;	
	}
}

void Cube::setVertexColour(int index, float red, float green, float blue, float alpha){
	vertexInterface->vertices->at(index).red = red;
	vertexInterface->vertices->at(index).green = green;
	vertexInterface->vertices->at(index).blue = blue;
	vertexInterface->vertices->at(index).alpha = alpha;
}

void Cube::update(void){
}
