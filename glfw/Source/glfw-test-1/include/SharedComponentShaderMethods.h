#pragma once

#include <GL/glew.h>

class NodeRenderable;
class RenderOptions;

namespace vox{
	class MatrixStack;
}

class SharedComponentShaderMethods {
public:
	static void configureLights(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
	static void configureMaterials(vox::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable);
};