#pragma once 

#include <vector>

#include "Shader.h"
#include "ShaderComponent.h"

class GeometryComponent;

class BaseComponentShader : public Shader{
public:	
	BaseComponentShader();
	explicit BaseComponentShader(std::vector<ShaderComponent *> _components);

	virtual ~BaseComponentShader();

	std::vector<ShaderComponent *> components;
	GeometryComponent * geometryComponent;

	void compileShader();

	virtual void configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable) override;
	virtual void clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
private:
	std::string buildVertexShader();
	std::string buildFragmentShader();
};