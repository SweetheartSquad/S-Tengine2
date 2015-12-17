#pragma once

#include <shader/ShaderComponent.h>

class ShaderComponentMVP : public ShaderComponent{
public:

	GLint modelUniformLocation, viewUniformLocation, projectionUniformLocation, mvpUniformLocation;

	ShaderComponentMVP(Shader * _shader);
	~ShaderComponentMVP();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void unload() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};