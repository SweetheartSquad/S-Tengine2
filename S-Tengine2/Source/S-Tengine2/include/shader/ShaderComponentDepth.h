#pragma once

#include <shader/ShaderComponent.h>

/**
* Simple shader component for rendering depth
*/
class ShaderComponentDepth : public ShaderComponent {
public:
	explicit ShaderComponentDepth(Shader * _shader);

	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

	void clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;

	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
};