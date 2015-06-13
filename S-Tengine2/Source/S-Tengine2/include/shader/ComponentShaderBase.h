#pragma once 

#include <vector>

#include "Shader.h"
#include "ShaderComponent.h"

class GeometryComponent;

class ComponentShaderBase : public Shader{
public:	
	GLint modelUniformLocation, viewUniformLocation, projectionUniformLocation, mvpUniformLocation;

	ComponentShaderBase(bool _autoRelease);
	explicit ComponentShaderBase(std::vector<ShaderComponent *> _components, bool _autoRelease);

	virtual ~ComponentShaderBase();

	GeometryComponent * geometryComponent;

	void compileShader();
	void addComponent(ShaderComponent * _component);
	ShaderComponent * getComponentAt(int idx);

	virtual void configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable) override;
	virtual void clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	
	virtual void load() override;
	virtual void unload() override;
private:
	std::vector<ShaderComponent *> components;
	std::string buildVertexShader();
	std::string buildFragmentShader();
};