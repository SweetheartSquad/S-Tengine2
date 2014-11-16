#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "NodeResource.h"

enum TypeName { PHONG, BLINN };

struct MaterialData {
	/** Name of material type (phong, blinn, etc) */
	TypeName type;
};

class Material : public NodeResource
{
public:
	Material(TypeName type, bool autoRelease);
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