#pragma once 

#include "ShaderComponent.h"

/******************************************************************************
*
* Adds Texture support to the shader that this component is added to 
*
* In order for textures to appear they must be added to rendered Mesh's texture vector
*
* See ShaderComponent for method descriptions
*
******************************************************************************/
class TextureShaderComponent : public ShaderComponent{
public:
	TextureShaderComponent();
	~TextureShaderComponent() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void configureUniforms(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};