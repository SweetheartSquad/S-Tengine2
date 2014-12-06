#pragma once 

#include <vector>

#include "Shader.h"
#include "ShaderComponent.h"

class BaseComponentShader : public Shader{
public:	
	BaseComponentShader();
	explicit BaseComponentShader(std::vector<ShaderComponent *> _components);

	~BaseComponentShader();

	std::vector<ShaderComponent *> components;

	void compileShader();

private:
	std::string buildVertexShader();
	std::string buildFragmentShader();
};