#include "Cube.h"
#include "VertexBuffer.h"

Cube::Cube(gmtl::Vec3f center, float size){
	
	scale(size, size, size);
	translate(center);

	//Top
	vertices->push_back(new gmtl::Vec3f(-1.f, 1.f, 1.f));
	vertices->push_back(new gmtl::Vec3f(-1.f, 1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, 1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, 1.f, 1.f));
	
	//Bottom
	vertices->push_back(new gmtl::Vec3f(-1.f, -1.f, 1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, -1.f, 1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, -1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(-1.f, -1.f, -1.f));

	//Front
	vertices->push_back(new gmtl::Vec3f(-1.f, 1.f, 1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, 1.f, 1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, -1.f, 1.f));
	vertices->push_back(new gmtl::Vec3f(-1.f, -1.f, 1.f));

	//Back
	vertices->push_back(new gmtl::Vec3f(-1.f, 1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, 1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, -1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(-1.f, -1.f, -1.f));

	//Left
	vertices->push_back(new gmtl::Vec3f(-1.f, 1.f, 1.f));
	vertices->push_back(new gmtl::Vec3f(-1.f, 1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(-1.f, -1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(-1.f, -1.f, 1.f));

	//Right
	vertices->push_back(new gmtl::Vec3f(1.f, 1.f, 1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, 1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, -1.f, -1.f));
	vertices->push_back(new gmtl::Vec3f(1.f, -1.f, 1.f));
	
	float *verts = static_cast<float*>(malloc(vertices->size()*sizeof(float)*3));
	int track = 0;
	for(int i=0; i<vertices->size(); i++){
		verts[track] = vertices->at(i)->getData()[0];
		verts[track+1] = vertices->at(i)->getData()[1];
		verts[track+2] = vertices->at(i)->getData()[2];
		track+=3;
	}
	vertexBuffer = new VertexBuffer(verts, sizeof(verts), GL_QUADS, 24, sizeof(GLfloat)*3);
	std::free(verts);
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
