#pragma once 

#include "ShaderComponent.h"

class ShaderComponentDiffuse : public ShaderComponent{
private:
	const bool doubleSided;
public:
	// if _doubleSided is true, the polygons can be lit from either side instead of just the front-face
	ShaderComponentDiffuse(ComponentShaderBase * _shader, bool _doubleSided = false);
	~ShaderComponentDiffuse() override;
	virtual std::string getVertexVariablesString() override;
	virtual std::string getFragmentVariablesString() override;
	virtual std::string getVertexBodyString() override;
	virtual std::string getFragmentBodyString() override;
	virtual std::string getOutColorMod() override;
	virtual void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
	virtual void clean(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};