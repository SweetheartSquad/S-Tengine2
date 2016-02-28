#pragma once

#include <shader/ShaderComponent.h>

/**
* Simple shader component for rendering depth
*/
class ShaderComponentDepth : public ShaderComponent {
public:
	explicit ShaderComponentDepth(ComponentShaderBase * _shader);

	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

	void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};