#include "Cube.h"

Cube::Cube(gmtl::Vec3f center, float size){
	
	scale(size, size, size);
	translate(center);
	//Top
	pushVert(-1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
	pushVert(-1.f, 1.f, -1.f, 1.f, 1.f, 1.f);
	pushVert(1.f, 1.f, -1.f, 1.f, 1.f, 1.f);
	pushVert(1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
	
	//Bottom
	pushVert(-1.f, -1.f, 1.f, 1.f, 1.f, 1.f);
	pushVert(1.f, -1.f, 1.f, 1.f, 1.f, 1.f);
	pushVert(1.f, -1.f, -1.f, 1.f, 1.f, 1.f);
	pushVert(-1.f, -1.f, -1.f, 1.f, 1.f, 1.f);

	//Front
	pushVert(-1.f, 1.f, 1.f, 0.f, 1.f, 0.f);
	pushVert(1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
	pushVert(1.f, -1.f, 1.f, 1.f, 1.f, 1.f);
	pushVert(-1.f, -1.f, 1.f, 1.f, 1.f, 1.f);

	//Back
	pushVert(-1.f, 1.f, -1.f, 0.f, 1.f, 1.f);
	pushVert(1.f, 1.f, -1.f, 1.f, 1.f, 1.f);
	pushVert(1.f, -1.f, -1.f, 1.f, 1.f, 1.f);
	pushVert(-1.f, -1.f, -1.f, 1.f, 1.f, 1.f);

	//Left
	pushVert(-1.f, 1.f, 1.f, 0.f, 0.f, 1.f);
	pushVert(-1.f, 1.f, -1.f, 1.f, 0.f, 1.f);
	pushVert(-1.f, -1.f, -1.f, 1.f, 0.f, 1.f);
	pushVert(-1.f, -1.f, 1.f, 1.f, 0.f, 1.f);

	//Right
	pushVert(1.f, 1.f, 1.f, 1.f, 0.f, 1.f);
	pushVert(1.f, 1.f, -1.f, 1.f, 0.f, 1.f);
	pushVert(1.f, -1.f, -1.f, 1.f, 0.f, 1.f);
	pushVert(1.f, -1.f, 1.f, 1.f, 0.f, 1.f);
	
	float *verts = vertsToFloatArray();
	vertexBuffer = new VertexBuffer(verts, 6*sizeof(float)*vertices->size(), GL_QUADS, GL_STATIC_DRAW, 24, 6*sizeof(float));
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
