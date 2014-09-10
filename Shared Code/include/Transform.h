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

	virtual void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix);
	virtual void update();

	void scale(float _scaleX, float _scaleY, float _scaleZ);
	void scale(glm::vec3 _scale);
	void scaleX(float _scaleX);
	void scaleY(float _scaleY);
	void scaleZ(float _scaleZ);

	void translate(float _translateX, float _translateY, float _translateZ);
	void translate(glm::vec3 _translate);
	void translateX(float _translateX);
	void translateY(float _translateY);
	void translateZ(float _translateZ);

	void rotate(glm::quat _rotation);
	void rotate(float _w, float _x, float _y, float _z);

	glm::mat4 getTranslationMatrix();
	glm::mat4 getScaleMatrix();
	glm::mat4 getRotationMatrix();
	glm::mat4 getModelMatrix();

	void addChild(Transform * _child);
	virtual void removeChildAtIndex(int _index);
	void setParent(Transform * _parent);
};