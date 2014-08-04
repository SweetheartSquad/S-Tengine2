#include "Cube.h"
#include "VertexBuffer.h"

Cube::Cube(gmtl::Vec3f center, float size){
	
	scale(size, size, size);
	translate(center);
	//Top
	vertices->push_back(vec6f(-1.f, 1.f, 1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(-1.f, 1.f, -1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(1.f, 1.f, -1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(1.f, 1.f, 1.f, 1.f, 1.f, 1.f));
	
	//Bottom
	vertices->push_back(vec6f(-1.f, -1.f, 1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(1.f, -1.f, 1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(1.f, -1.f, -1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(-1.f, -1.f, -1.f, 1.f, 1.f, 1.f));

	//Front
	vertices->push_back(vec6f(-1.f, 1.f, 1.f, 0.f, 1.f, 0.f));
	vertices->push_back(vec6f(1.f, 1.f, 1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(1.f, -1.f, 1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(-1.f, -1.f, 1.f, 1.f, 1.f, 1.f));

	//Back
	vertices->push_back(vec6f(-1.f, 1.f, -1.f, 0.f, 1.f, 1.f));
	vertices->push_back(vec6f(1.f, 1.f, -1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(1.f, -1.f, -1.f, 1.f, 1.f, 1.f));
	vertices->push_back(vec6f(-1.f, -1.f, -1.f, 1.f, 1.f, 1.f));

	//Left
	vertices->push_back(vec6f(-1.f, 1.f, 1.f, 0.f, 0.f, 1.f));
	vertices->push_back(vec6f(-1.f, 1.f, -1.f, 1.f, 0.f, 1.f));
	vertices->push_back(vec6f(-1.f, -1.f, -1.f, 1.f, 0.f, 1.f));
	vertices->push_back(vec6f(-1.f, -1.f, 1.f, 1.f, 0.f, 1.f));

	//Right
	vertices->push_back(vec6f(1.f, 1.f, 1.f, 1.f, 0.f, 1.f));
	vertices->push_back(vec6f(1.f, 1.f, -1.f, 1.f, 0.f, 1.f));
	vertices->push_back(vec6f(1.f, -1.f, -1.f, 1.f, 0.f, 1.f));
	vertices->push_back(vec6f(1.f, -1.f, 1.f, 1.f, 0.f, 1.f));
	
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
