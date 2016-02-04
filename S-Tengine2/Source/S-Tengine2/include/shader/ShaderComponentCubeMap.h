#pragma once

#include <shader/ShaderComponentMVP.h>

class ShaderComponentCubeMap : public ShaderComponentMVP{
public:

	GLint cubemapUniformLocation;

	ShaderComponentCubeMap(ComponentShaderBase * _shader);
	~ShaderComponentCubeMap();
	
	virtual std::string getFragmentVariablesString() override;
	virtual std::string getVertexBodyString() override;
	virtual std::string getFragmentBodyString() override;
	virtual std::string getOutColorMod() override;
	
	virtual void load() override;
	virtual void unload() override;

	virtual void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};