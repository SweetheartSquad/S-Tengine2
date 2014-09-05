#pragma once

#include <iostream>
#include <vector>
#include <iostream>

#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Node.h"

//a generic transform node
class Transform: public Node{
public:
	glm::vec3 translationVector;
	glm::vec3 scaleVector;
	glm::quat orientation;

	std::vector<Transform *> children;
	Transform * parent;


	Transform(void);
	virtual ~Transform(void);

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

	void addChild(Transform * child);
	virtual void removeChildAtIndex(int index);
	void setParent(Transform * parent);
};