#pragma once 

#include <vector>

#include "Shader.h"
#include "ShaderComponent.h"

class GeometryComponent;

class BaseComponentShader : public Shader{
public:	
	BaseComponentShader(bool _autoRelease);
	explicit BaseComponentShader(std::vector<ShaderComponent *> _components, bool _autoRelease);

	virtual ~BaseComponentShader();

	GeometryComponent * geometryComponent;

	void compileShader();
	void addComponent(ShaderComponent * _component);

	virtual void configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable) override;
	virtual void clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
private:
	std::vector<ShaderComponent *> components;
	std::string buildVertexShader();
	std::string buildFragmentShader();
};