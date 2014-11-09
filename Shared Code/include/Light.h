#pragma once

#include <glm/glm.hpp>

#include "Transform.h"
#include "Node.h"
#include "Vox.h"
#include "MatrixStack.h"

struct LightData {
	glm::vec3 position;
	glm::vec3 intensities;
	float ambientCoefficient;
	float attenuation;
};

/**A basic light node. Stores a reference to a transform and a struct containing the position and color of the light */
class Light : public Node{
public:
	/** Reference to this entity's transform */
	Transform * transform;

	Light();
	explicit Light(Transform * _transform);
	~Light();

	/**
	* Pushes model matrix stack,
	* Applies the model matrix of transform,
	* Pops model matrix stack
	*/
	virtual void draw(MatrixStack * _matrixStack);
	void update();

	/**
	* The data struct which is passed to the shader
	* This should be copied in the shader
	*/
	LightData data;
};