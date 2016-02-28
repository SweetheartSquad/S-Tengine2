#pragma once

#include <shader/ShaderComponent.h>

class ShaderComponentMVP : public ShaderComponent{
public:

	GLint modelUniformLocation, viewUniformLocation, projectionUniformLocation, mvpUniformLocation;

	ShaderComponentMVP(ComponentShaderBase * _shader);
	~ShaderComponentMVP();
	
	virtual std::string getVertexVariablesString() override;
	virtual std::string getFragmentVariablesString() override;
	virtual std::string getVertexBodyString() override;
	virtual std::string getFragmentBodyString() override;
	virtual std::string getOutColorMod() override;
	void load() override;
	void unload() override;
	virtual void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};