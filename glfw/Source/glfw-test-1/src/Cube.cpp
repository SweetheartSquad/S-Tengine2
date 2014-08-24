#include "Cube.h"

Cube::Cube(glm::vec3 center, float size){
	
	transform->scale(size, size, size);
	transform->translate(center);
	//Top
	pushVert(Vertex(-1.f, 1.f, 1.f));
	pushVert(Vertex(-1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, 1.f, 1.f));
	
	//Bottom
	pushVert(Vertex(-1.f, -1.f, 1.f));
	pushVert(Vertex(1.f, -1.f, 1.f));
	pushVert(Vertex(1.f, -1.f, -1.f));
	pushVert(Vertex(-1.f, -1.f, -1.f));

	//Front
	pushVert(Vertex(-1.f, 1.f, 1.f));
	pushVert(Vertex(1.f, 1.f, 1.f));
	pushVert(Vertex(1.f, -1.f, 1.f));
	pushVert(Vertex(-1.f, -1.f, 1.f));

	//Back
	pushVert(Vertex(-1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, -1.f, -1.f));
	pushVert(Vertex(-1.f, -1.f, -1.f));

	//Left
	pushVert(Vertex(-1.f, 1.f, 1.f));
	pushVert(Vertex(-1.f, 1.f, -1.f));
	pushVert(Vertex(-1.f, -1.f, -1.f));
	pushVert(Vertex(-1.f, -1.f, 1.f));

	//Right
	pushVert(Vertex(1.f, 1.f, 1.f));
	pushVert(Vertex(1.f, 1.f, -1.f));
	pushVert(Vertex(1.f, -1.f, -1.f));
	pushVert(Vertex(1.f, -1.f, 1.f));
	
	Vertex *vertsPtr = vertices->data();
	vertexBuffer = new VertexBuffer(vertsPtr, sizeof(Vertex)*vertices->size(), GL_QUADS, GL_STATIC_DRAW, 24, sizeof(Vertex));
}

Cube::~Cube(void)
{
}

void Cube::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix){
	Entity::draw(projectionMatrix, viewMatrix);
}

void Cube::setColour(float red, float green, float blue, float alpha)
{
	for(int i=0; i<24; i++)
	{
		vertices->at(i).red = red;
		vertices->at(i).green = green;
		vertices->at(i).blue = blue;
		vertices->at(i).alpha = alpha;	
	}
}

void Cube::setLeftColour(float red, float green, float blue, float alpha)
{

	for(int i=16; i<20; i++)
	{
		vertices->at(i).red = red;
		vertices->at(i).green = green;
		vertices->at(i).blue = blue;
		vertices->at(i).alpha = alpha;	
	}

}

void Cube::setRightColour(float red, float green, float blue, float alpha)
{
	for(int i=20; i<24; i++)
	{
		vertices->at(i).red = red;
		vertices->at(i).green = green;
		vertices->at(i).blue = blue;
		vertices->at(i).alpha = alpha;	
	}
}

void Cube::setTopColour(float red, float green, float blue, float alpha)
{
	for(int i=0; i<4; i++)
	{
		vertices->at(i).red = red;
		vertices->at(i).green = green;
		vertices->at(i).blue = blue;
		vertices->at(i).alpha = alpha;	
	}
}

void Cube::setBottomColour(float red, float green, float blue, float alpha)
{
	for(int i=0; i<8; i++)
	{
		vertices->at(i).red = red;
		vertices->at(i).green = green;
		vertices->at(i).blue = blue;
		vertices->at(i).alpha = alpha;	
	}
}

void Cube::setFrontColour(float red, float green, float blue, float alpha)
{
	for(int i=8; i<12; i++)
	{
		vertices->at(i).red = red;
		vertices->at(i).green = green;
		vertices->at(i).blue = blue;
		vertices->at(i).alpha = alpha;	
	}
}

void Cube::setBackColour(float red, float green, float blue, float alpha)
{
	for(int i=12; i<16; i++)
	{
		vertices->at(i).red = red;
		vertices->at(i).green = green;
		vertices->at(i).blue = blue;
		vertices->at(i).alpha = alpha;	
	}
}

void Cube::setVertexColour(int index, float red, float green, float blue, float alpha)
{
	vertices->at(index).red = red;
	vertices->at(index).green = green;
	vertices->at(index).blue = blue;
	vertices->at(index).alpha = alpha;
}

void Cube::update(void)
{

}
