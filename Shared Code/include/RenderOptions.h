#pragma once 

#include <vector>
#include "GL/glew.h"
#include <glm/glm.hpp>

class Shader;
class Light;

class RenderOptions{
	
public:

	RenderOptions();
	RenderOptions(Shader * _shader, std::vector<Light *> * _lights);

	~RenderOptions();

	std::vector<Light*> * lights;
	Shader * shader;
	GLuint shadowMapTextureId;

	glm::mat4 cameraPos;
};