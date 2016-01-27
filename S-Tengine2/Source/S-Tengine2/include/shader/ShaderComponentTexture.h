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
class ShaderComponentTexture : public ShaderComponent{
private:
	GLint texNumLoc, texSamLoc;
	unsigned long int numTextures;
public:
	float  alphaDiscardThreshold;
	
	// if _alphaDiscardThreshold is > 0, pixels which have an alpha of less than _alphaDiscardThreshold will be discarded in the fragment shader
	ShaderComponentTexture(ComponentShaderBase * _shader, float _alphaDiscardThreshold = -1);
	~ShaderComponentTexture() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void unload() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	virtual void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};