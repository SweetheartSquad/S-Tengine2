#pragma once

#include "ShaderComponent.h"
#include <glm/detail/type_vec3.hpp>

class ShaderComponentText : public ShaderComponent{
public:

	glm::vec3 color;

	ShaderComponentText(Shader * _shader);
	~ShaderComponentText() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	virtual void clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};