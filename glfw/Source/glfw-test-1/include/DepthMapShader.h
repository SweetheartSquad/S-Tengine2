#pragma once

#include "Shader.h"

class DepthMapShader : public Shader{
public:
	DepthMapShader(bool _autoRelease);

	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

private:
	std::string getVertString();
	std::string getFragString();
};
