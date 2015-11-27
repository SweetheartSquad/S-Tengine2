#pragma once

#include <shader/ShaderComponent.h>

class ShaderComponentDepthOffset : public ShaderComponent{
public:
	ShaderComponentDepthOffset(Shader * _shader);
	~ShaderComponentDepthOffset();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

private:
	float depthOffset;
	unsigned long long currentCycle;
};