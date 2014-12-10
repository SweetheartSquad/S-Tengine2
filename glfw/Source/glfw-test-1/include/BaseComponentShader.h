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

	virtual void configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable) override;

private:
	std::string buildVertexShader();
	std::string buildFragmentShader();
};