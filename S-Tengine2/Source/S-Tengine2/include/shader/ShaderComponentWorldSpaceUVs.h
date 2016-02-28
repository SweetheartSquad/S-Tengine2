#pragma once 

#include "ShaderComponent.h"

/******************************************************************************
*
* Sets the UV coordinates to the worldspace XY coordinates
*
******************************************************************************/
class ShaderComponentWorldSpaceUVs : public ShaderComponent{
private:
	GLint xMultiplierLoc, yMultiplierLoc;
public:
	float xMultiplier;
	float yMultiplier;

	ShaderComponentWorldSpaceUVs(ComponentShaderBase * _shader);
	~ShaderComponentWorldSpaceUVs() override;
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	virtual void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};