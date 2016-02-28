#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "node/NodeResource.h"
#include "Sweet.h"

struct MaterialData {
	// shininess is an exponent for a number between 0 and 1
	float shininess;
	glm::vec3 specularColor;

	MaterialData(float _shininess, glm::vec3 _specularColor);
};

class Material : public NodeResource
{
public:
	Material(float shininess, glm::vec3 specularColour, bool autoRelease);
	~Material();

	/** Material doesn't need to load anything */
	virtual void load() override;
	virtual void unload() override;
	
	/**
	* The data struct which is passed to the shader
	* This should be copied in the shader
	*/
	MaterialData data;
};