#pragma once 

#include <vector>

#include "Shader.h"
#include "ShaderComponent.h"

class BaseShader : public Shader{
public:	
	BaseShader();
	explicit BaseShader(std::vector<ShaderComponent *> * _components);

	~BaseShader();

	std::vector<ShaderComponent *> * components;

	void compileShader();

private:
	std::string buildVertexShader();
	std::string buildFragmentShader();
};