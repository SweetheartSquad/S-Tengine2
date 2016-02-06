#pragma once

#include "GeometryComponent.h"

class ShaderComponentVoxel : public GeometryComponent{
public:
	ShaderComponentVoxel(Shader * _shader);
	std::string getGeometryShader() override;
	std::string getVertexVariablesString() override;
	std::string getVertexBodyString() override;
	void configureUniforms(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable) override;
};