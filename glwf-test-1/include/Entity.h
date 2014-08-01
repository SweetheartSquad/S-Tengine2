
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

class Entity
{

public:

	float x;
	float y;
	float z;

	gmtl::Vec3f *translationVector;
	gmtl::Quatf *rotation;
	gmtl::Vec3f *scaleVector;
	std::vector<gmtl::Vec<float,3>*> *vertices;
	VertexBuffer *vertexBuffer;
	std::vector<Entity*> *children;
	Entity *parent;

	Entity(void);
	//Entity(float x, float y, float z);
	~Entity(void);

	virtual void draw();
	virtual void update();
	
	void scale(float scaleX, float scaleY, float scaleZ);
	void scale(gmtl::Vec3f scale);
	void scaleX(float scaleX);
	void scaleY(float scaleY);
	void scaleZ(float scaleZ);
	
	void translate(float translateX, float translateY, float translateZ);
	void translate(gmtl::Vec3f translate);
	void translateX(float translateX);
	void translateY(float translateY);
	void translateZ(float translateZ);

	void rotate(gmtl::Quatf rotation);
	void rotate(float w, float x, float y, float z);
	
	float* vertsToFloatArray();
};

