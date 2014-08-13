#include "Cube.h"

Cube::Cube(glm::vec3 center, float size){
	
	scale(size, size, size);
	translate(center);
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

void Cube::draw(void){
	Entity::draw();
}

void Cube::update(void)
{

}
