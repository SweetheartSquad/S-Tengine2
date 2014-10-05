#pragma once

#include <glm/glm.hpp>

#include "Transform.h"
#include "Node.h"
#include "Vox.h"

struct LightData {
	glm::vec3 position;
	glm::vec3 intensities;
};

//A basic light node. Stores a reference to a transform and a struct containing the position and color of the light
class Light : public Node{
public:
	Transform * transform;			// Reference to this entity's transform
	
	Light(Transform * _transform = nullptr);
	~Light();

	/*
	Pushes model matrix stack,
	Applies the model matrix of transform,
	Pops model matrix stack
	*/
	virtual void draw(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix);

	LightData data;
};