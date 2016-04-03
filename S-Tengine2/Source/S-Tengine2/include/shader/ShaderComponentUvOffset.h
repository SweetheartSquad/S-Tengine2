#pragma once 

#include "ShaderComponent.h"

/******************************************************************************
*
* Sets the UV coordinates to the worldspace XY coordinates
*
******************************************************************************/
class ShaderComponentUvOffset : public ShaderComponent{
private:
	GLint xOffsetLoc, yOffsetLoc;
public:
	float xOffset;
	float yOffset;

	ShaderComponentUvOffset(ComponentShaderBase * _shader);
	~ShaderComponentUvOffset() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	virtual void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};