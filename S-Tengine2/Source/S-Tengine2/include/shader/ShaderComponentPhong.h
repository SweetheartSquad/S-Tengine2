#pragma once

#include "ShaderComponent.h"

/******************************************************************************
*
* Adds phong material support to the shader that this component is added to.
*
* In order for the phong effect to occur the Mesh that is being rendered must
* have a material applied to it
*
* See ShaderComponent for method descriptions
*
*******************************************************************************/

class ShaderComponentPhong : public ShaderComponent{
public:
	ShaderComponentPhong(Shader * _shader);
	~ShaderComponentPhong() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	void clean(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};