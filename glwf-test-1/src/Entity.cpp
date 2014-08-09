#include "Entity.h"


Entity::Entity(void){
	x = 0;
	y = 0;
	z = 0;
	this->vertices = new std::vector<gmtl::Vec<float,6>*>;
	this->translationVector = new glm::vec3(0.0f,0.0f,0.0f);
	this->scaleVector = new glm::vec3(1.f,1.f,1.f);
	this->rotationVector = new glm::quat();
	this->children = new std::vector<Entity*>();
}

Entity::~Entity(void)
{
}

void Entity::draw()
{	
	glTranslatef(translationVector->x, translationVector->y, translationVector->z);
	//gmtl::AxisAnglef r;
	//gmtl::set(r, *rotationVector);
	glm::quat rotationT = glm::angleAxis(rotationVector->w, glm::vec3(rotationVector->x, rotationVector->y, rotationVector->z));
	//glRotatef(rotationT.w, rotationT.x, rotationT.y, rotationT.z);
	glScalef(scaleVector->x, scaleVector->y, scaleVector->z);
}

void Entity::update()
{
}

void Entity::scale(float scaleX, float scaleY, float scaleZ)
{

	scaleVector->x *= scaleX;
	scaleVector->y *= scaleY;
	scaleVector->z *= scaleZ;
}

void Entity::scale(glm::vec3 scale)
{
	*scaleVector *= scale;
}

void Entity::scaleX(float scaleX)
{
	scaleVector->x *= scaleX;
}

void Entity::scaleY(float scaleY)
{
	scaleVector->y *= scaleY;
}

void Entity::scaleZ(float scaleZ)
{
	scaleVector->z *= scaleZ;
}

void Entity::translate(float translateX, float translateY, float translateZ)
{
	translationVector->x += translateX;
	translationVector->y += translateY;
	translationVector->z += translateZ;
}

void Entity::translate(glm::vec3 translate)
{
	*translationVector += translate.x;
}

void Entity::translateX(float translateX)
{
	translationVector->x += translateX;
}

void Entity::translateY(float translateY)
{
	translationVector->y += translateY;
}

void Entity::translateZ(float translateZ)
{
	translationVector->z += translateZ;
}

void Entity::rotate(glm::quat rotation){
	*this->rotationVector = rotation*(*this->rotationVector);
}

glm::mat4 Entity::getTranslationMatrix()
{
	return glm::translate(*translationVector);
}

glm::mat4 Entity::getScaleMatrix()
{
	return glm::scale(*scaleVector);
}

glm::mat4 Entity::getRotationMatrix()
{
	return glm::rotate(rotationVector->w, glm::vec3(rotationVector->x, rotationVector->y, rotationVector->z));
}

void Entity::rotate(float w, float x, float y, float z){
	glm::quat r;
	r = glm::rotate(r, w, glm::vec3(x,y,z));
	rotate(r);
}

void Entity::addChild(Entity* child)
{
	children->push_back(child);
}

void Entity::removeChildAtIndex(int index)
{
	children->erase(children->begin()+index-1);
}

float* Entity::vertsToFloatArray(){	
	int track = 0;
	float *verts = new float[vertices->size()*6]();
	for(int i=0; i<vertices->size(); i++){
		verts[track] = vertices->at(i)->getData()[0];
		verts[track+1] = vertices->at(i)->getData()[1];
		verts[track+2] = vertices->at(i)->getData()[2];
		verts[track+3] = vertices->at(i)->getData()[3];
		verts[track+4] = vertices->at(i)->getData()[4];
		verts[track+5] = vertices->at(i)->getData()[5];
		track+=6;
	}
	return verts;
}

void Entity::pushVert(float x, float y, float z, float red, float green, float blue)
{
	gmtl::Vec<float, 6>* v = new gmtl::Vec<float, 6>();
	v->mData[0] = x;
	v->mData[1] = y;
	v->mData[2] = z;
	v->mData[3] = red;
	v->mData[4] = green;
	v->mData[5] = blue;
	vertices->push_back(v);
}