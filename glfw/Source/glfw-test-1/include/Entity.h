
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
#include "Vertex.h"


class Entity
{

public:

	glm::vec3 *translationVector;
	glm::vec3 *scaleVector;
	glm::quat *orientation;

	VertexBuffer *vertexBuffer;
	std::vector<Entity*> *children;
	Entity *parent;
	ShaderInterface *shader;

	std::vector<Vertex>*vertices;

	Entity(void);
	virtual ~Entity(void);

	virtual void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
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
	glm::mat4 getModelMatrix();

	void addChild(Entity* child);
	void removeChildAtIndex(int index);

	void Entity::pushVert(Vertex vertex);
};

