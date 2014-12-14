#pragma once

#include "GeometryComponent.h"

class VoxelComponent : public GeometryComponent
{
public:
	VoxelComponent();
	std::string getGeometryShader() override;
	std::string getVertexVariablesString() override;
	std::string getVertexBodyString() override;
	void configureUniforms(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption,  NodeRenderable* _nodeRenderable) override;
};