#pragma once

#include "Shader.h"

class DepthMapShader : public Shader{
public:
	explicit DepthMapShader(bool _autoRelease);

	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

private:
	std::string getVertString();
	std::string getFragString();
};
