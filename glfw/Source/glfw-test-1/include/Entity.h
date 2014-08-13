
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <gmtl/Vec.h>
#include <gmtl/AxisAngle.h>
#include <gmtl/VecBase.h>
#include <gmtl/VecExprMeta.h>
#include <gmtl/VecOps.h>
#include <gmtl/QuatOps.h>
#include <gmtl/gmtl.h>
#include <iostream>
#include <gmtl\Generate.h>
#include "VertexBuffer.h"
#include "ShaderInterface.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


typedef struct Vertex
{
	float x,y,z;
	float red,green,blue,alpha;
	float nx, ny, nz;
	float u,v;
	Vertex(float x, float y, float z, 
					  float red, float green, float blue, float alpha, 
					  float normalX, float normalY, float normalZ, 
					  float u, float v)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->alpha = alpha;
		this->nx = normalX;
		this->ny = normalY;
		this->nz = normalZ;
		this->u = u;
		this->v = v;
	}

	Vertex(float x, float y, float z, 
				float red, float green, float blue, float alpha)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->alpha = alpha;
		this->nx = 0;
		this->ny = 0;
		this->nz = 0;
		this->u = 0;
		this->v = 0;
	}

	Vertex(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->red = 1.0f;
		this->green = 1.0f;
		this->blue = 1.0f;
		this->alpha = 1.0f;
		this->nx = 0;
		this->ny = 0;
		this->nz = 0;
		this->u = 0;
		this->v = 0;
	}
};

class Entity
{

public:

	float x;
	float y;
	float z;

	glm::vec3 *translationVector;
	glm::vec3 *scaleVector;
	glm::quat *rotationVector;

	VertexBuffer *vertexBuffer;
	std::vector<Entity*> *children;
	Entity *parent;
	ShaderInterface *shader;

	std::vector<Vertex>*vertices;

	Entity(void);
	virtual ~Entity(void);

	virtual void draw();
	virtual void update();
	
	void scale(float scaleX, float scaleY, float scaleZ);
	void scale(glm::vec3 scale);
	void scaleX(float scaleX);
	void scaleY(float scaleY);
	void scaleZ(float scaleZ);
	
	void translate(float translateX, float translateY, float translateZ);
	void translate(glm::vec3 translate);
	void translateX(float translateX);
	void translateY(float translateY);
	void translateZ(float translateZ);

	void rotate(glm::quat rotation);
	void rotate(float w, float x, float y, float z);
	
	glm::mat4 getTranslationMatrix();
	glm::mat4 getScaleMatrix();
	glm::mat4 getRotationMatrix();

	void addChild(Entity* child);
	void removeChildAtIndex(int index);

	void Entity::pushVert(Vertex vertex);
};

