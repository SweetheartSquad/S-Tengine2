#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "NodeResource.h"
#include "Vox.h"

struct MaterialData {
	float shininess;
	glm::vec3 specularColor;
};

class Material : public NodeResource
{
public:
	Material(float shininess, glm::vec3 specularColour, bool autoRelease);
	~Material();

	/** Material doesn't need to load anything */
	virtual void load();
	virtual void unload();
	
	/**
	* The data struct which is passed to the shader
	* This should be copied in the shader
	*/
	MaterialData data;
};