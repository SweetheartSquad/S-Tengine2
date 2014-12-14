#pragma once

#include "ShaderComponent.h"

/******************************************************************************
*
* Adds shadow support to the shader that this component is added to
*
* In order for shadows to be rendered the renderShadow method of scene must be 
* called. This is because this method will render a depth map which is used to determine
* how the shadows will be rendered
*
* See ShaderComponent for method descriptions
*
*******************************************************************************/
class ShadowShaderComponent : public ShaderComponent{
public:
	ShadowShaderComponent();
	~ShadowShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};