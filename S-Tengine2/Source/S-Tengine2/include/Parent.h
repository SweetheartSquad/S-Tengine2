#pragma once

#include <node\Node.h>

#include <glm\glm.hpp>

class Transform;

class Parent : public Node{
public:
	Transform * transform;

	virtual void makeCumulativeModelMatrixDirty();
	bool cumulativeModelMatrixDirty;
	glm::vec3 worldPos;
	glm::mat4 cumulativeModelMatrix;


	Parent(Transform * _transform);
	~Parent();
};