#include "Entity.h"


Entity::Entity(void){
	x = 0;
	y = 0;
	z = 0;
	this->vertices = new std::vector<gmtl::Vec<float,3>*>;
	this->translationVector = new gmtl::Vec3f(0.0f,0.0f,0.0f);
	this->scaleVector = new gmtl::Vec3f(1.f,1.f,1.f);
	this->rotation = new gmtl::Quatf();
}

Entity::~Entity(void)
{
}

void Entity::draw()
{	
	float* trans = translationVector->getData(); 
	glTranslatef(trans[0], trans[1], trans[2]);
	gmtl::AxisAnglef r;
	gmtl::set(r, *rotation);
	glRotatef(r.getAngle()*180/3.1415926, r.getAxis().getData()[0], r.getAxis().getData()[1], r.getAxis().getData()[2]);
	glScalef(scaleVector->getData()[0], scaleVector->getData()[1], scaleVector->getData()[2]);
}

void Entity::update()
{
}

void Entity::scale(float scaleX, float scaleY, float scaleZ)
{
	scaleVector->getData()[0] *= scaleX;
	scaleVector->getData()[1] *= scaleY;
	scaleVector->getData()[2] *= scaleZ;
}

void Entity::scale(gmtl::Vec3f scale)
{
	scaleVector->getData()[0] *= scale[0];
	scaleVector->getData()[1] *= scale[1];
	scaleVector->getData()[2] *= scale[2];
}

void Entity::scaleX(float scaleX)
{
	scaleVector->getData()[0] *= scaleX;
}

void Entity::scaleY(float scaleY)
{
	scaleVector->getData()[1] *= scaleY;
}

void Entity::scaleZ(float scaleZ)
{
	scaleVector->getData()[2] *= scaleZ;
}

void Entity::translate(float translateX, float translateY, float translateZ)
{
	translationVector->getData()[0] += translateX;
	translationVector->getData()[1] += translateY;
	translationVector->getData()[2] += translateZ;
}

void Entity::translate(gmtl::Vec3f translate)
{
	*translationVector += translate;
}

void Entity::translateX(float translateX)
{
	translationVector->getData()[0] += translateX;
}

void Entity::translateY(float translateY)
{
	translationVector->getData()[1] += translateY;
}

void Entity::translateZ(float translateZ)
{
	translationVector->getData()[2] += translateZ;
}

void Entity::rotate(gmtl::Quatf rotation){
	*this->rotation = rotation*(*this->rotation);
}

void Entity::rotate(float w, float x, float y, float z){
	gmtl::Quatf r;
	gmtl::set(r, gmtl::AxisAnglef(w/180*3.1415926,x,y,z));
	rotate(r);
}

float* Entity::vertsToFloatArray(){	
	int track = 0;
	float *verts = new float[vertices->size()*3]();
	for(int i=0; i<vertices->size(); i++){
		verts[track] = vertices->at(i)->getData()[0];
		verts[track+1] = vertices->at(i)->getData()[1];
		verts[track+2] = vertices->at(i)->getData()[2];
		track+=3;
	}
	return verts;
}